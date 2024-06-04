- Disable automouse, make mouse active while you hold key

- change tap-hold: I_ESC's hold action must trigger on 130ms timeout expire, not on release

- figure out what to do with numbers. Changing to a separate layer to use numbers\arrows is not really 
  convenient. First solutions should probably be combos or momentary toggle

- bring three combos (copy and select) from moonlander

- fix a `ni` issue (nullify timer)

- enable scroll mode on dpad press and hold

- moving language select this far is a bad move

- enable f11 (kitty expansion as a consequence)

- change mouse button to a combo of three

- for japanese, enable selecting a language accurately, like on moonlander. Combo invocation of LGUI does not seem to work

- under the middle finger on numbers layer do some more often used shift + left\right arrow combinations. 
  specifically used for messages and selecting text.

- combo of three doesn't really feel good. I would like just a less frequent combo of two.
  feels like `lk` can be a good candidate

- shift and ctrl on the left hand in the same places as on the right

## Not flashed

## You are here

- increase tapping term on right pinky finger
    - tapping term is not exactly what I needed. even if tapping term is low, I still press the next
    key BEFORE releasing the key under pinkey finger. This automatically counts as held. I need to just register
    custom keycode and release it exacly the same way as I_ESC, the problem is exactly the same. Just refactor your 
    timers a bit so they are extensible)

- maybe rebind ctrl + h to backspace entirely? - unfortunately, ctrl + h has a meaning (in vim mainly) and will have 
  to stay ctrl-h. I don't have means in my keyboard to know what Vim mode am I currently in.
  just rebind in lua if the problem is with neovim only?
- https://docs.qmk.fm/feature_key_overrides#ko-make-basic-modifiers-key-replacement

- find a transistor and pray my button will work, so I can shift QAZ column one down (author was correct,
  I should not have raised the column for no reason lol. Chesterton's fence).

- rgb brightness (does not work for some reason, wait for the last transistor)

- sometimes pressing left dpad for too long causes random very long sequences of wwwww rrrrrr. Figure out why.
    - after extended testing, pressing dpad feels uncomfortable for some reason. Remap scroll-hold to something
      else entirely

## Think a bit harder

- think of a special usage pattern to do relative jumps which would not require arrow keys
  caveat: 155 is technically indistinguishable from 15k, right? So the differentiation will have to come 
  from the change in my behaviour, i will have to supply this bit of info. perhaps an additional way of 
  entering the numbers layer (== effectively a separate jump layer)?

## Just thoughts

- you have two mod-tap keys: left ctrl and shift. If you set tapping term to smth like 100ms,
  you should theoretically not notice in any usage where ctrl or shift are held, but this technically
  frees two buttons for quick tapping

- I should acknowledge that numbers \ symbols absolutely break my everything and are NOT NICE to use.
    - when typing numbers, I often need - or _.
