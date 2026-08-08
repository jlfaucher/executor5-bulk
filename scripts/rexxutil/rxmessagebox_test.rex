/*
Recommendations from Claude.ai Sonnet 4.6

A few things worth checking during your tests:

Emoji rendering — make sure the text you pass contains some emojis
(e.g. L"Hello 😀✅🎉") to confirm the colored rendering is working as expected
with "Segoe UI Emoji".

Sizing edge cases to try:

A very short message (single word)
→ the caption or button bar should set the minimum width

A very long message (many words)
→ the dialog should grow wide enough

A multi-line message with \n
→ each line should be measured independently

An empty caption (L"")
→ shouldn't crash

Button sets — exercise all six combinations (MB_OK, MB_OKCANCEL, MB_YESNO,
MB_YESNOCANCEL, MB_ABORTRETRYIGNORE, MB_RETRYCANCEL) to confirm they all lay out
correctly and return the right IDOK/IDYES/etc. value.

Icons — try each of MB_ICONINFORMATION, MB_ICONWARNING, MB_ICONERROR,
MB_ICONQUESTION and also no icon at all.

Return value — confirm the value returned by CustomMessageBoxW matches the
button the user clicked, since that's what ooRexx will use to branch logic.
*/

use arg testCase
if arg(1, "O") then testCase = 1

signal value testCase
1:


/*********
* Emojis *
*********/

-- No title
10:
say RxMessageBox('10: Père Noël 🎅', /*title*/, /*button*/, /*icon*/, /*option*/, 65001)
say RxMessageBox('10: Père Noël 🎅', /*title*/, /*button*/, /*icon*/, "CUSTOM", 65001)

-- Long text
11:
say RxMessageBox('11: ✅ Long text with Père Noël 🎅 and Mère Noël 🤶', /*title*/, /*button*/, /*icon*/, /*option*/, 65001)
say RxMessageBox('11: ✅ Long text with Père Noël 🎅 and Mère Noël 🤶', /*title*/, /*button*/, /*icon*/, "CUSTOM", 65001)

-- Long title
12:
say RxMessageBox("Long title", '12: ✅ Long title with Père Noël 🎅 and Mère Noël 🤶', /*button*/, /*icon*/, /*option*/, 65001)
say RxMessageBox("Long title", '12: ✅ Long title with Père Noël 🎅 and Mère Noël 🤶', /*button*/, /*icon*/, "CUSTOM", 65001)

-- Long multiline text
-- When displaying long multi-line text with many emojis using the CUSTOM option,
-- there is a noticeable lag between the dialog box display and the text display.
-- AI can't do better. It's definitely not me who can make this better.
13:
text = .resources~multiline_stress_1
say RxMessageBox(text, "13: Long multiline text", /*button*/, /*icon*/, /*option*/, 65001)
say RxMessageBox(text, "13: Long multiline text", /*button*/, /*icon*/, "CUSTOM", 65001)


/**********
* Buttons *
**********/

-- Ok, Cancel
20:
text = .resources~multiline_stress_2
say RxMessageBox(text, "20: Ok, Cancel", "OKCANCEL", /*icon*/, /*option*/, 65001)
say RxMessageBox(text, "20: Ok, Cancel", "OKCANCEL", /*icon*/, "CUSTOM", 65001)

-- Abort, Retry, Ignore
21:
text = .resources~multiline_stress_2
say RxMessageBox(text, "21: Abort, Retry, Ignore", "ABORTRETRYIGNORE", /*icon*/, /*option*/, 65001)
say RxMessageBox(text, "21: Abort, Retry, Ignore", "ABORTRETRYIGNORE", /*icon*/, "CUSTOM", 65001)

-- Yes, No, Cancel
22:
text = .resources~multiline_stress_2
say RxMessageBox(text, "22: Yes, No, Cancel", "YESNOCANCEL", /*icon*/, /*option*/, 65001)
say RxMessageBox(text, "22: Yes, No, Cancel", "YESNOCANCEL", /*icon*/, "CUSTOM", 65001)

-- Yes, No
23:
text = .resources~multiline_stress_2
say RxMessageBox(text, "23: Yes, No", "YESNO", /*icon*/, /*option*/, 65001)
say RxMessageBox(text, "23: Yes, No", "YESNO", /*icon*/, "CUSTOM", 65001)

-- Retry Cancel
24:
text = .resources~multiline_stress_2
say RxMessageBox(text, "24: Retry, Cancel", "RETRYCANCEL", /*icon*/, /*option*/, 65001)
say RxMessageBox(text, "24: Retry, Cancel", "RETRYCANCEL", /*icon*/, "CUSTOM", 65001)

-- Cancel, Try Again, Continue
25:
text = .resources~multiline_stress_2
say RxMessageBox(text, "25: Cancel, Try Again, Continue", "CANCELTRYCONTINUE", /*icon*/, /*option*/, 65001)
say RxMessageBox(text, "25: Cancel, Try Again, Continue", "CANCELTRYCONTINUE", /*icon*/, "CUSTOM", 65001)

-- HELP button (not documented but supported)
26:
text = .resources~multiline_stress_2
say RxMessageBox(text, "26: HELP button", "HELP", /*icon*/, /*option*/, 65001)
say RxMessageBox(text, "26: HELP button", "HELP", /*icon*/, "CUSTOM", 65001)


/***************************
* Buttons with HELP option *
***************************/

-- Ok, Cancel, Help
120:
text = .resources~multiline_stress_2
say RxMessageBox(text, "120: Ok, Cancel, Help", "OKCANCEL", /*icon*/, "HELP", 65001)
say RxMessageBox(text, "120: Ok, Cancel, Help", "OKCANCEL", /*icon*/, "HELP CUSTOM", 65001)

-- Abort, Retry, Ignore, Help
121:
text = .resources~multiline_stress_2
say RxMessageBox(text, "121: Abort, Retry, Ignore, Help", "ABORTRETRYIGNORE", /*icon*/, "HELP", 65001)
say RxMessageBox(text, "121: Abort, Retry, Ignore, Help", "ABORTRETRYIGNORE", /*icon*/, "HELP CUSTOM", 65001)

-- Yes, No, Cancel, Help
122:
text = .resources~multiline_stress_2
say RxMessageBox(text, "122: Yes, No, Cancel, Help", "YESNOCANCEL", /*icon*/, "HELP", 65001)
say RxMessageBox(text, "122: Yes, No, Cancel, Help", "YESNOCANCEL", /*icon*/, "HELP CUSTOM", 65001)

-- Yes, No, Help
123:
text = .resources~multiline_stress_2
say RxMessageBox(text, "123: Yes, No, Help", "YESNO", /*icon*/, "HELP", 65001)
say RxMessageBox(text, "123: Yes, No, Help", "YESNO", /*icon*/, "HELP CUSTOM", 65001)

-- Retry Cancel, Help
124:
text = .resources~multiline_stress_2
say RxMessageBox(text, "124: Retry, Cancel, Help", "RETRYCANCEL", /*icon*/, "HELP", 65001)
say RxMessageBox(text, "124: Retry, Cancel, Help", "RETRYCANCEL", /*icon*/, "HELP CUSTOM", 65001)

-- Cancel, Try Again, Continue, Help
125:
text = .resources~multiline_stress_2
say RxMessageBox(text, "125: Cancel, Try Again, Continue, Help", "CANCELTRYCONTINUE", /*icon*/, "HELP", 65001)
say RxMessageBox(text, "125: Cancel, Try Again, Continue, Help", "CANCELTRYCONTINUE", /*icon*/, "HELP CUSTOM", 65001)

-- HELP button (not documented but supported) with HELP option
126:
text = .resources~multiline_stress_2
say RxMessageBox(text, "126: HELP button with HELP option", "HELP", /*icon*/, "HELP", 65001)
say RxMessageBox(text, "126: HELP button with HELP option", "HELP", /*icon*/, "HELP CUSTOM", 65001)


/************************
* Default button option *
************************/

-- Abort (default), Retry, Ignore, Help
200:
/* DEFBUTTON1 not supported... Seriously???
say RxMessageBox("Default button: Abort", "200: Abort, Retry, Ignore, Help", "ABORTRETRYIGNORE", /*icon*/, "DEFBUTTON1 HELP", 65001)
say RxMessageBox("Default button: Abort", "200: Abort, Retry, Ignore, Help", "ABORTRETRYIGNORE", /*icon*/, "DEFBUTTON1 HELP CUSTOM", 65001)
*/

-- Abort, Retry (default), Ignore, Help
201:
say RxMessageBox("Default button: Retry", "201: Abort, Retry, Ignore, Help", "ABORTRETRYIGNORE", /*icon*/, "DEFBUTTON2 HELP", 65001)
say RxMessageBox("Default button: Retry", "201: Abort, Retry, Ignore, Help", "ABORTRETRYIGNORE", /*icon*/, "DEFBUTTON2 HELP CUSTOM", 65001)

-- Abort, Retry, Ignore (default), Help
202:
say RxMessageBox("Default button: Ignore", "202: Abort, Retry, Ignore, Help", "ABORTRETRYIGNORE", /*icon*/, "DEFBUTTON3 HELP", 65001)
say RxMessageBox("Default button: Ignore", "202: Abort, Retry, Ignore, Help", "ABORTRETRYIGNORE", /*icon*/, "DEFBUTTON3 HELP CUSTOM", 65001)

-- Abort, Retry, Ignore , Help (default)
203:
say RxMessageBox("Default button: Help", "203: Abort, Retry, Ignore, Help", "ABORTRETRYIGNORE", /*icon*/, "DEFBUTTON4 HELP", 65001)
say RxMessageBox("Default button: Help", "203: Abort, Retry, Ignore, Help", "ABORTRETRYIGNORE", /*icon*/, "DEFBUTTON4 HELP CUSTOM", 65001)


/**************
* Icon option *
**************/

-- Icon: NONE
300:
text = .resources~multiline_stress_1
say RxMessageBox(text, "300: Icon: NONE", "OKCANCEL", "NONE", "HELP", 65001)
say RxMessageBox(text, "300: Icon: NONE", "OKCANCEL", "NONE", "HELP CUSTOM", 65001)

-- Icon: INFORMATION
301:
text = .resources~multiline_stress_1
say RxMessageBox(text, "301: Icon: INFORMATION", "OKCANCEL", "INFORMATION", "HELP", 65001)
say RxMessageBox(text, "301: Icon: INFORMATION", "OKCANCEL", "INFORMATION", "HELP CUSTOM", 65001)

-- Icon: ASTERISK
302:
text = .resources~multiline_stress_1
say RxMessageBox(text, "302: Icon: ASTERISK", "OKCANCEL", "ASTERISK", "HELP", 65001)
say RxMessageBox(text, "302: Icon: ASTERISK", "OKCANCEL", "ASTERISK", "HELP CUSTOM", 65001)

-- Icon: WARNING
303:
text = .resources~multiline_stress_1
say RxMessageBox(text, "303: Icon: WARNING", "OKCANCEL", "WARNING", "HELP", 65001)
say RxMessageBox(text, "303: Icon: WARNING", "OKCANCEL", "WARNING", "HELP CUSTOM", 65001)

-- Icon: EXCLAMATION
304:
text = .resources~multiline_stress_1
say RxMessageBox(text, "304: Icon: EXCLAMATION", "OKCANCEL", "EXCLAMATION", "HELP", 65001)
say RxMessageBox(text, "304: Icon: EXCLAMATION", "OKCANCEL", "EXCLAMATION", "HELP CUSTOM", 65001)

-- Icon: ERROR
305:
text = .resources~multiline_stress_2
say RxMessageBox(text, "305: Icon: ERROR", "OKCANCEL", "ERROR", "HELP", 65001)
say RxMessageBox(text, "305: Icon: ERROR", "OKCANCEL", "ERROR", "HELP CUSTOM", 65001)

-- Icon: HAND
306:
text = .resources~multiline_stress_2
say RxMessageBox(text, "306: Icon: HAND", "OKCANCEL", "HAND", "HELP", 65001)
say RxMessageBox(text, "306: Icon: HAND", "OKCANCEL", "HAND", "HELP CUSTOM", 65001)

-- Icon: STOP
307:
text = .resources~multiline_stress_2
say RxMessageBox(text, "307: Icon: STOP", "OKCANCEL", "STOP", "HELP", 65001)
say RxMessageBox(text, "307: Icon: STOP", "OKCANCEL", "STOP", "HELP CUSTOM", 65001)

-- Icon: QUESTION
307:
text = .resources~multiline_stress_2
say RxMessageBox(text, "308: Icon: QUESTION", "OKCANCEL", "QUESTION", "HELP", 65001)
say RxMessageBox(text, "308: Icon: QUESTION", "OKCANCEL", "QUESTION", "HELP CUSTOM", 65001)

-- Icon: QUERY
308:
text = .resources~multiline_stress_2
say RxMessageBox(text, "308: Icon: QUERY", "OKCANCEL", "QUERY", "HELP", 65001)
say RxMessageBox(text, "308: Icon: QUERY", "OKCANCEL", "QUERY", "HELP CUSTOM", 65001)


--------------------------------------------------------------------------------
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


--------------------------------------------------------------------------------
::RESOURCE multiline_stress_2
# Big List of Naughty Strings
# https://github.com/minimaxir/big-list-of-naughty-strings

#	Special Unicode Characters Union
#
#	A super string recommended by VMware Inc. Globalization Team: can effectively cause rendering issues or character-length issues to validate product globalization readiness.
#
#	表          CJK_UNIFIED_IDEOGRAPHS (U+8868)
#	ポ          KATAKANA LETTER PO (U+30DD)
#	あ          HIRAGANA LETTER A (U+3042)
#	A           LATIN CAPITAL LETTER A (U+0041)
#	鷗          CJK_UNIFIED_IDEOGRAPHS (U+9DD7)
#	Œ           LATIN SMALL LIGATURE OE (U+0153)
#	é           LATIN SMALL LETTER E WITH ACUTE (U+00E9)
#	Ｂ           FULLWIDTH LATIN CAPITAL LETTER B (U+FF22)
#	逍          CJK_UNIFIED_IDEOGRAPHS (U+900D)
#	Ü           LATIN SMALL LETTER U WITH DIAERESIS (U+00FC)
#	ß           LATIN SMALL LETTER SHARP S (U+00DF)
#	ª           FEMININE ORDINAL INDICATOR (U+00AA)
#	ą           LATIN SMALL LETTER A WITH OGONEK (U+0105)
#	ñ           LATIN SMALL LETTER N WITH TILDE (U+00F1)
#	丂          CJK_UNIFIED_IDEOGRAPHS (U+4E02)
#	㐀          CJK Ideograph Extension A, First (U+3400)
#	𠀀          CJK Ideograph Extension B, First (U+20000)

表ポあA鷗ŒéＢ逍Üßªąñ丂㐀𠀀
::END

