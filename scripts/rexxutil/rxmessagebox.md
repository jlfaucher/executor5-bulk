<p align="right" style="font-size:10px;">
<a style="font-size:10px;" href="https://github.com/jlfaucher/executor5-bulk/blob/main/scripts/rexxutil/rxmessagebox.md">Markdown</a><br/>
<a style="font-size:10px;" href="https://html-preview.github.io/?url=https://github.com/jlfaucher/executor5-bulk/blob/main/scripts/rexxutil/rxmessagebox.html">HTML</a>
</p>

# Rexx Utilities (RexxUtil)

## RxMessageBox (Windows only)

For users (like me) who prefer not to enable the Windows `UTF-8` beta option:  
Added `codepage` and `translateflags` parameters to `RxMessageBox`.

Motivated by:
[https://sourceforge.net/p/oorexx/discussion/408478/thread/ec4123a46d/](https://sourceforge.net/p/oorexx/discussion/408478/thread/ec4123a46d/)  
2025 Mar 22 [oorexx:discussions] Output text in Russian in `RxMessageBox()`

    RxMessageBox(text [, title] [, button] [,icon] [, option]    [, codepage [, translateflags]])
                                                     *updated*       *new*          *new*

    new CUSTOM option.  
    new HELP option.  
    codepage = text and title code page.  
    translateflags = flags for SysToUnicode.

Functionalities:

- If `codePage` is omitted or is an empty string, `text` and `title` (which should
  be `ANSI` strings) are converted from `ACP` to `Unicode` by the system.

- If `codePage` is `"UNICODE"`, `text` and `title` (which should be `UTF-16` strings)
  are displayed as-is.

- For any other value of `codepage`, `text` and `title` (which should be strings
  encoded in `codepage`) are converted from `codepage` to `Unicode` using the `Rexx`
  utility `SysToUnicode`.

- The standard `MessageBoxW` is used by default to display `Unicode` strings.
  If the `CUSTOM` option is specified then a custom `MessageBoxW` is used.
  The custom `MessageBoxW` displays emojis in colors using the `"Segoe UI Emoji"`
  font that carries the full-color `COLR/CPAL` emoji glyphs on Windows 10+.
  The emojis in the caption are still displayed in black and white.

- If the `HELP` option is specified then a `Help` button is added to the message
  box. This is applicable to any button style, so more general than the `HELP`
  button style.

- The `HELP` button style is not documented, but is supported.
  The message box contains two push buttons: `OK` and `Help`.

The behavior of the custom `MessageBoxW` is different from the standard
`MessageBoxW` regarding the effect of the `Help` button:

- Standard `MessageBoxW`: When the user clicks the `Help` button or presses `F1`,
  the system sends a `WM_HELP` message to the owner window. The message box is
  not closed. Since `RxMessageBox` creates a message box with no owner window,
  the `WM_HELP` message is not sent.

- Custom `MessageBoxW`: When the user clicks the `Help` button, the message box
  is closed and the value `9` is returned. The `F1` key has no effect.

When displaying long multi-line text with many emojis using the `CUSTOM` option,
there is a noticeable lag between the dialog box display and the text display.
AI can't do better. It's definitely not me who can make this better.

The default modality is `MB_APPLMODAL`.

The following settings are not supported, and there is no plan to support them:

    Modality:
        MB_SYSTEMMODAL
        MB_TASKMODAL

    Other options:
        MB_SETFOREGROUND
        MB_DEFAULT_DESKTOP_ONLY
        MB_TOPMOST
        MB_RIGHT
        MB_RTLREADING
        MB_SERVICE_NOTIFICATION

### Test case 13 - Long multiline text

#### Code

```rexx
text = .resources~multiline_stress_1
say RxMessageBox(text, "13: Long multiline text", /*button*/, /*icon*/, /*option*/, 65001)
say RxMessageBox(text, "13: Long multiline text", /*button*/, /*icon*/, "CUSTOM", 65001)

::RESOURCE multiline_stress_1
# Big List of Naughty Strings
# https://github.com/minimaxir/big-list-of-naughty-strings

#	Emoji
#
#	Strings which contain Emoji; should be the same behavior as two-byte characters, but not always

😍
👩🏽
👨‍🦰 👨🏿‍🦰 👨‍🦱 👨🏿‍🦱 🦹🏿‍♂️
👾 🙇 💁 🙅 🙆 🙋 🙎 🙍
🐵 🙈 🙉 🙊
❤️ 💔 💌 💕 💞 💓 💗 💖 💘 💝 💟 💜 💛 💚 💙
✋🏿 💪🏿 👐🏿 🙌🏿 👏🏿 🙏🏿
👨‍👩‍👦 👨‍👩‍👧‍👦 👨‍👨‍👦 👩‍👩‍👧 👨‍👦 👨‍👧‍👦 👩‍👦 👩‍👧‍👦
🚾 🆒 🆓 🆕 🆖 🆗 🆙 🏧
0️⃣ 1️⃣ 2️⃣ 3️⃣ 4️⃣ 5️⃣ 6️⃣ 7️⃣ 8️⃣ 9️⃣ 🔟

#       Regional Indicator Symbols
#
#       Regional Indicator Symbols can be displayed differently across
#       fonts, and have a number of special behaviors

🇺🇸🇷🇺🇸 🇦🇫🇦🇲🇸
🇺🇸🇷🇺🇸🇦🇫🇦🇲
🇺🇸🇷🇺🇸🇦

#	Unicode font
#
#	Strings which contain bold/italic/etc. versions of normal characters

Ｔｈｅ ｑｕｉｃｋ ｂｒｏｗｎ ｆｏｘ ｊｕｍｐｓ ｏｖｅｒ ｔｈｅ ｌａｚｙ ｄｏｇ
𝐓𝐡𝐞 𝐪𝐮𝐢𝐜𝐤 𝐛𝐫𝐨𝐰𝐧 𝐟𝐨𝐱 𝐣𝐮𝐦𝐩𝐬 𝐨𝐯𝐞𝐫 𝐭𝐡𝐞 𝐥𝐚𝐳𝐲 𝐝𝐨𝐠
𝕿𝖍𝖊 𝖖𝖚𝖎𝖈𝖐 𝖇𝖗𝖔𝖜𝖓 𝖋𝖔𝖝 𝖏𝖚𝖒𝖕𝖘 𝖔𝖛𝖊𝖗 𝖙𝖍𝖊 𝖑𝖆𝖟𝖞 𝖉𝖔𝖌
𝑻𝒉𝒆 𝒒𝒖𝒊𝒄𝒌 𝒃𝒓𝒐𝒘𝒏 𝒇𝒐𝒙 𝒋𝒖𝒎𝒑𝒔 𝒐𝒗𝒆𝒓 𝒕𝒉𝒆 𝒍𝒂𝒛𝒚 𝒅𝒐𝒈
𝓣𝓱𝓮 𝓺𝓾𝓲𝓬𝓴 𝓫𝓻𝓸𝔀𝓷 𝓯𝓸𝔁 𝓳𝓾𝓶𝓹𝓼 𝓸𝓿𝓮𝓻 𝓽𝓱𝓮 𝓵𝓪𝔃𝔂 𝓭𝓸𝓰
𝕋𝕙𝕖 𝕢𝕦𝕚𝕔𝕜 𝕓𝕣𝕠𝕨𝕟 𝕗𝕠𝕩 𝕛𝕦𝕞𝕡𝕤 𝕠𝕧𝕖𝕣 𝕥𝕙𝕖 𝕝𝕒𝕫𝕪 𝕕𝕠𝕘
𝚃𝚑𝚎 𝚚𝚞𝚒𝚌𝚔 𝚋𝚛𝚘𝚠𝚗 𝚏𝚘𝚡 𝚓𝚞𝚖𝚙𝚜 𝚘𝚟𝚎𝚛 𝚝𝚑𝚎 𝚕𝚊𝚣𝚢 𝚍𝚘𝚐
⒯⒣⒠ ⒬⒰⒤⒞⒦ ⒝⒭⒪⒲⒩ ⒡⒪⒳ ⒥⒰⒨⒫⒮ ⒪⒱⒠⒭ ⒯⒣⒠ ⒧⒜⒵⒴ ⒟⒪⒢
::END

```

#### Standard Dialog box

```rexx
say RxMessageBox(text, "13: Long multiline text", /*button*/, /*icon*/, /*option*/, 65001)

```

<p align="center">
<img width="60%" src="rxmessagebox_test.rex%20-%2013.png" alt="Standard Dialog box">
</p>


#### Custom Dialog box

```rexx
say RxMessageBox(text, "13: Long multiline text", /*button*/, /*icon*/, "CUSTOM", 65001)

```

<p align="center">
<img width="60%" src="rxmessagebox_test.rex%20-%2013%20(custom).png" alt="Custom Dialog box">
</p>

### Other test cases

Script [`rxmessagebox_test.rex`](https://github.com/jlfaucher/executor5-bulk/blob/main/scripts/rexxutil/rxmessagebox_test.rex)  

