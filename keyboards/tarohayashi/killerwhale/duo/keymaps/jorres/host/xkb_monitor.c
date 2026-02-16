// xkb_monitor - Monitors X11 XKB layout group changes and prints layout name to stdout.
// Build: gcc -o xkb_monitor xkb_monitor.c -lX11 -lxkbfile
// Usage: ./xkb_monitor   (prints layout name on each change, e.g. "us" or "ru")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/XKBlib.h>
#include <X11/extensions/XKBrules.h>

// Returns the layout name for the given XKB group by parsing the
// comma-separated layout string from XKB rules.
static const char *get_layout_for_group(Display *dpy, unsigned char group) {
    XkbRF_VarDefsRec vd;
    if (!XkbRF_GetNamesProp(dpy, NULL, &vd) || !vd.layout) {
        return "unknown";
    }

    // Walk the comma-separated layout list to find the entry at index `group`
    static char buf[64];
    strncpy(buf, vd.layout, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    char *tok = strtok(buf, ",");
    for (unsigned i = 0; i < group && tok; i++) {
        tok = strtok(NULL, ",");
    }
    return tok ? tok : "unknown";
}

int main(void) {
    Display *dpy = XOpenDisplay(NULL);
    if (!dpy) {
        fprintf(stderr, "xkb_monitor: cannot open display\n");
        return 1;
    }

    int xkb_event_base, xkb_error_base;
    if (!XkbQueryExtension(dpy, NULL, &xkb_event_base, &xkb_error_base, NULL, NULL)) {
        fprintf(stderr, "xkb_monitor: XKB extension not available\n");
        XCloseDisplay(dpy);
        return 1;
    }

    XkbSelectEvents(dpy, XkbUseCoreKbd, XkbStateNotifyMask, XkbStateNotifyMask);
    XkbSelectEventDetails(dpy, XkbUseCoreKbd, XkbStateNotify,
                          XkbGroupStateMask, XkbGroupStateMask);

    // Print initial layout
    XkbStateRec state;
    XkbGetState(dpy, XkbUseCoreKbd, &state);
    printf("%s\n", get_layout_for_group(dpy, state.group));
    fflush(stdout);

    unsigned char prev_group = state.group;

    for (;;) {
        XEvent ev;
        XNextEvent(dpy, &ev);

        if (ev.type == xkb_event_base + XkbEventCode) {
            XkbEvent *xkb_ev = (XkbEvent *)&ev;
            if (xkb_ev->any.xkb_type == XkbStateNotify) {
                unsigned char group = xkb_ev->state.group;
                if (group != prev_group) {
                    printf("%s\n", get_layout_for_group(dpy, group));
                    fflush(stdout);
                    prev_group = group;
                }
            }
        }
    }

    XCloseDisplay(dpy);
    return 0;
}
