- My findings about the unicode input:
  - it seems to be impossible to activate the unicode input being on Russian layout. 
    I would expect a setting that I could change from ctrl+shift+u to ctrl+shift+г (not 
    even as much as change, but rather add). Even better, not assuming the `add` functionality, 
    I could have changed it to ctrl+shift+! and it would have been available on both my layouts!
  - I have been able to make unicode input work on English layout in Telegram, browser and in kitty 
    (kitty required some additional configuration, but this is done one time only and is not destructive
    and does not blink as other applications, thank for that).
    - but all of this honestly in vain, because when I open telegram and type in russian, I would like the 
      same key to produce @. But there is NO @ key in Russian, and neither there is a way to trigger unicode!
  - changing the opening sequence in QMK will not help, because all processing is done

  - changing layouts to type a specific character and then changing back again seems too slow, the latency is visible (CHECK)

  - you can only stay on English, specify russian characters as unicode when keyboard changes, but this would
    destroy the ability to type on a laptop keyboard...

  - what the fuck is multi lingual layouts? I would like my computer to interpret english and russian both 
    at a time!!!
    - does not seem that such one exists... see https://en.wikipedia.org/wiki/List_of_QWERTY_keyboard_language_variants
    - even fucking english-estonian layout does exist to accomodate two fucking estonian letters. Sheesh.

  - Day2 research topic:
    - custom layouts? xmodmap? xkb?
       https://www.jwz.org/xkeycaps/

  - The most interesting thing: it's FUN and I would DEFINITELY have one more research session like this.
