-- https://sourceforge.net/p/oorexx/bugs/2060/

tc = 0
signal on syntax

-- 0 arg + 1 extra
1:  tc += 1; say "tc =" tc; call r0; say result
2:  tc += 1; say "tc =" tc; call r0 1; say result

-- 1 arg + 1 extra
3:  tc += 1; say "tc =" tc; call r1; say result
4:  tc += 1; say "tc =" tc; call r1 , ; say result
5:  tc += 1; say "tc =" tc; call r1 , 2; say result
6:  tc += 1; say "tc =" tc; call r1 1; say result
7:  tc += 1; say "tc =" tc; call r1 1, ; say result
8:  tc += 1; say "tc =" tc; call r1 1, 2; say result

-- 1 arg + 1 extra
9:  tc += 1; say "tc =" tc; call rd; say result
10: tc += 1; say "tc =" tc; call rd , ; say result
11: tc += 1; say "tc =" tc; call rd , 2; say result
12: tc += 1; say "tc =" tc; call rd 1; say result
13: tc += 1; say "tc =" tc; call rd 1, ; say result
14: tc += 1; say "tc =" tc; call rd 1, 2; say result

-- 2 args + 1 extra
15: tc += 1; say "tc =" tc; call r00; say result
16: tc += 1; say "tc =" tc; call r00 , ; say result
17: tc += 1; say "tc =" tc; call r00 , , ; say result
18: tc += 1; say "tc =" tc; call r00 , , 3; say result
19: tc += 1; say "tc =" tc; call r00 , 2; say result
20: tc += 1; say "tc =" tc; call r00 , 2, ; say result
21: tc += 1; say "tc =" tc; call r00 , 2, 3; say result
22: tc += 1; say "tc =" tc; call r00 1; say result
23: tc += 1; say "tc =" tc; call r00 1, ; say result
24: tc += 1; say "tc =" tc; call r00 1, , ; say result
25: tc += 1; say "tc =" tc; call r00 1, , 3; say result
26: tc += 1; say "tc =" tc; call r00 1, 2; say result
27: tc += 1; say "tc =" tc; call r00 1, 2, ; say result
28: tc += 1; say "tc =" tc; call r00 1, 2, 3; say result

-- 2 args + 1 extra
-- r01 (labels 29: to 42:)
29:  tc += 1; say "tc =" tc; call r01; say result
30:  tc += 1; say "tc =" tc; call r01 , ; say result
31:  tc += 1; say "tc =" tc; call r01 , , ; say result
32:  tc += 1; say "tc =" tc; call r01 , , 3; say result
33:  tc += 1; say "tc =" tc; call r01 , 2; say result
34:  tc += 1; say "tc =" tc; call r01 , 2, ; say result
35:  tc += 1; say "tc =" tc; call r01 , 2, 3; say result
36:  tc += 1; say "tc =" tc; call r01 1; say result
37:  tc += 1; say "tc =" tc; call r01 1, ; say result
38:  tc += 1; say "tc =" tc; call r01 1, , ; say result
39:  tc += 1; say "tc =" tc; call r01 1, , 3; say result
40:  tc += 1; say "tc =" tc; call r01 1, 2; say result
41:  tc += 1; say "tc =" tc; call r01 1, 2, ; say result
42:  tc += 1; say "tc =" tc; call r01 1, 2, 3; say result

-- r0d (labels 43: to 56:)
43:  tc += 1; say "tc =" tc; call r0d; say result
44:  tc += 1; say "tc =" tc; call r0d , ; say result
45:  tc += 1; say "tc =" tc; call r0d , , ; say result
46:  tc += 1; say "tc =" tc; call r0d , , 3; say result
47:  tc += 1; say "tc =" tc; call r0d , 2; say result
48:  tc += 1; say "tc =" tc; call r0d , 2, ; say result
49:  tc += 1; say "tc =" tc; call r0d , 2, 3; say result
50:  tc += 1; say "tc =" tc; call r0d 1; say result
51:  tc += 1; say "tc =" tc; call r0d 1, ; say result
52:  tc += 1; say "tc =" tc; call r0d 1, , ; say result
53:  tc += 1; say "tc =" tc; call r0d 1, , 3; say result
54:  tc += 1; say "tc =" tc; call r0d 1, 2; say result
55:  tc += 1; say "tc =" tc; call r0d 1, 2, ; say result
56:  tc += 1; say "tc =" tc; call r0d 1, 2, 3; say result

-- r10 (labels 57: to 70:)
57:  tc += 1; say "tc =" tc; call r10; say result
58:  tc += 1; say "tc =" tc; call r10 , ; say result
59:  tc += 1; say "tc =" tc; call r10 , , ; say result
60:  tc += 1; say "tc =" tc; call r10 , , 3; say result
61:  tc += 1; say "tc =" tc; call r10 , 2; say result
62:  tc += 1; say "tc =" tc; call r10 , 2, ; say result
63:  tc += 1; say "tc =" tc; call r10 , 2, 3; say result
64:  tc += 1; say "tc =" tc; call r10 1; say result
65:  tc += 1; say "tc =" tc; call r10 1, ; say result
66:  tc += 1; say "tc =" tc; call r10 1, , ; say result
67:  tc += 1; say "tc =" tc; call r10 1, , 3; say result
68:  tc += 1; say "tc =" tc; call r10 1, 2; say result
69:  tc += 1; say "tc =" tc; call r10 1, 2, ; say result
70:  tc += 1; say "tc =" tc; call r10 1, 2, 3; say result

-- r11 (labels 71: to 84:)
71:  tc += 1; say "tc =" tc; call r11; say result
72:  tc += 1; say "tc =" tc; call r11 , ; say result
73:  tc += 1; say "tc =" tc; call r11 , , ; say result
74:  tc += 1; say "tc =" tc; call r11 , , 3; say result
75:  tc += 1; say "tc =" tc; call r11 , 2; say result
76:  tc += 1; say "tc =" tc; call r11 , 2, ; say result
77:  tc += 1; say "tc =" tc; call r11 , 2, 3; say result
78:  tc += 1; say "tc =" tc; call r11 1; say result
79:  tc += 1; say "tc =" tc; call r11 1, ; say result
80:  tc += 1; say "tc =" tc; call r11 1, , ; say result
81:  tc += 1; say "tc =" tc; call r11 1, , 3; say result
82:  tc += 1; say "tc =" tc; call r11 1, 2; say result
83:  tc += 1; say "tc =" tc; call r11 1, 2, ; say result
84:  tc += 1; say "tc =" tc; call r11 1, 2, 3; say result

-- r1d (labels 85: to 98:)
85:  tc += 1; say "tc =" tc; call r1d; say result
86:  tc += 1; say "tc =" tc; call r1d , ; say result
87:  tc += 1; say "tc =" tc; call r1d , , ; say result
88:  tc += 1; say "tc =" tc; call r1d , , 3; say result
89:  tc += 1; say "tc =" tc; call r1d , 2; say result
90:  tc += 1; say "tc =" tc; call r1d , 2, ; say result
91:  tc += 1; say "tc =" tc; call r1d , 2, 3; say result
92:  tc += 1; say "tc =" tc; call r1d 1; say result
93:  tc += 1; say "tc =" tc; call r1d 1, ; say result
94:  tc += 1; say "tc =" tc; call r1d 1, , ; say result
95:  tc += 1; say "tc =" tc; call r1d 1, , 3; say result
96:  tc += 1; say "tc =" tc; call r1d 1, 2; say result
97:  tc += 1; say "tc =" tc; call r1d 1, 2, ; say result
98:  tc += 1; say "tc =" tc; call r1d 1, 2, 3; say result

-- rd0 (labels 99: to 112:)
99:  tc += 1; say "tc =" tc; call rd0; say result
100: tc += 1; say "tc =" tc; call rd0 , ; say result
101: tc += 1; say "tc =" tc; call rd0 , , ; say result
102: tc += 1; say "tc =" tc; call rd0 , , 3; say result
103: tc += 1; say "tc =" tc; call rd0 , 2; say result
104: tc += 1; say "tc =" tc; call rd0 , 2, ; say result
105: tc += 1; say "tc =" tc; call rd0 , 2, 3; say result
106: tc += 1; say "tc =" tc; call rd0 1; say result
107: tc += 1; say "tc =" tc; call rd0 1, ; say result
108: tc += 1; say "tc =" tc; call rd0 1, , ; say result
109: tc += 1; say "tc =" tc; call rd0 1, , 3; say result
110: tc += 1; say "tc =" tc; call rd0 1, 2; say result
111: tc += 1; say "tc =" tc; call rd0 1, 2, ; say result
112: tc += 1; say "tc =" tc; call rd0 1, 2, 3; say result

-- rd1 (labels 113: to 126:)
113: tc += 1; say "tc =" tc; call rd1; say result
114: tc += 1; say "tc =" tc; call rd1 , ; say result
115: tc += 1; say "tc =" tc; call rd1 , , ; say result
116: tc += 1; say "tc =" tc; call rd1 , , 3; say result
117: tc += 1; say "tc =" tc; call rd1 , 2; say result
118: tc += 1; say "tc =" tc; call rd1 , 2, ; say result
119: tc += 1; say "tc =" tc; call rd1 , 2, 3; say result
120: tc += 1; say "tc =" tc; call rd1 1; say result
121: tc += 1; say "tc =" tc; call rd1 1, ; say result
122: tc += 1; say "tc =" tc; call rd1 1, , ; say result
123: tc += 1; say "tc =" tc; call rd1 1, , 3; say result
124: tc += 1; say "tc =" tc; call rd1 1, 2; say result
125: tc += 1; say "tc =" tc; call rd1 1, 2, ; say result
126: tc += 1; say "tc =" tc; call rd1 1, 2, 3; say result

-- rdd (labels 127: to 140:)
127: tc += 1; say "tc =" tc; call rdd; say result
128: tc += 1; say "tc =" tc; call rdd , ; say result
129: tc += 1; say "tc =" tc; call rdd , , ; say result
130: tc += 1; say "tc =" tc; call rdd , , 3; say result
131: tc += 1; say "tc =" tc; call rdd , 2; say result
132: tc += 1; say "tc =" tc; call rdd , 2, ; say result
133: tc += 1; say "tc =" tc; call rdd , 2, 3; say result
134: tc += 1; say "tc =" tc; call rdd 1; say result
135: tc += 1; say "tc =" tc; call rdd 1, ; say result
136: tc += 1; say "tc =" tc; call rdd 1, , ; say result
137: tc += 1; say "tc =" tc; call rdd 1, , 3; say result
138: tc += 1; say "tc =" tc; call rdd 1, 2; say result
139: tc += 1; say "tc =" tc; call rdd 1, 2, ; say result
140: tc += 1; say "tc =" tc; call rdd 1, 2, 3; say result

-- 3 args + 1 extra
-- r000 (labels 141: to 170:)
141: tc += 1; say "tc =" tc; call r000; say result
142: tc += 1; say "tc =" tc; call r000 , ; say result
143: tc += 1; say "tc =" tc; call r000 , , ; say result
144: tc += 1; say "tc =" tc; call r000 , , , ; say result
145: tc += 1; say "tc =" tc; call r000 , , , 4; say result
146: tc += 1; say "tc =" tc; call r000 , , 3; say result
147: tc += 1; say "tc =" tc; call r000 , , 3, ; say result
148: tc += 1; say "tc =" tc; call r000 , , 3, 4; say result
149: tc += 1; say "tc =" tc; call r000 , 2; say result
150: tc += 1; say "tc =" tc; call r000 , 2, ; say result
151: tc += 1; say "tc =" tc; call r000 , 2, , ; say result
152: tc += 1; say "tc =" tc; call r000 , 2, , 4; say result
153: tc += 1; say "tc =" tc; call r000 , 2, 3; say result
154: tc += 1; say "tc =" tc; call r000 , 2, 3, ; say result
155: tc += 1; say "tc =" tc; call r000 , 2, 3, 4; say result
156: tc += 1; say "tc =" tc; call r000 1; say result
157: tc += 1; say "tc =" tc; call r000 1, ; say result
158: tc += 1; say "tc =" tc; call r000 1, , ; say result
159: tc += 1; say "tc =" tc; call r000 1, , , ; say result
160: tc += 1; say "tc =" tc; call r000 1, , , 4; say result
161: tc += 1; say "tc =" tc; call r000 1, , 3; say result
162: tc += 1; say "tc =" tc; call r000 1, , 3, ; say result
163: tc += 1; say "tc =" tc; call r000 1, , 3, 4; say result
164: tc += 1; say "tc =" tc; call r000 1, 2; say result
165: tc += 1; say "tc =" tc; call r000 1, 2, ; say result
166: tc += 1; say "tc =" tc; call r000 1, 2, , ; say result
167: tc += 1; say "tc =" tc; call r000 1, 2, , 4; say result
168: tc += 1; say "tc =" tc; call r000 1, 2, 3; say result
169: tc += 1; say "tc =" tc; call r000 1, 2, 3, ; say result
170: tc += 1; say "tc =" tc; call r000 1, 2, 3, 4; say result

-- r001 (labels 171: to 200:)
171: tc += 1; say "tc =" tc; call r001; say result
172: tc += 1; say "tc =" tc; call r001 , ; say result
173: tc += 1; say "tc =" tc; call r001 , , ; say result
174: tc += 1; say "tc =" tc; call r001 , , , ; say result
175: tc += 1; say "tc =" tc; call r001 , , , 4; say result
176: tc += 1; say "tc =" tc; call r001 , , 3; say result
177: tc += 1; say "tc =" tc; call r001 , , 3, ; say result
178: tc += 1; say "tc =" tc; call r001 , , 3, 4; say result
179: tc += 1; say "tc =" tc; call r001 , 2; say result
180: tc += 1; say "tc =" tc; call r001 , 2, ; say result
181: tc += 1; say "tc =" tc; call r001 , 2, , ; say result
182: tc += 1; say "tc =" tc; call r001 , 2, , 4; say result
183: tc += 1; say "tc =" tc; call r001 , 2, 3; say result
184: tc += 1; say "tc =" tc; call r001 , 2, 3, ; say result
185: tc += 1; say "tc =" tc; call r001 , 2, 3, 4; say result
186: tc += 1; say "tc =" tc; call r001 1; say result
187: tc += 1; say "tc =" tc; call r001 1, ; say result
188: tc += 1; say "tc =" tc; call r001 1, , ; say result
189: tc += 1; say "tc =" tc; call r001 1, , , ; say result
190: tc += 1; say "tc =" tc; call r001 1, , , 4; say result
191: tc += 1; say "tc =" tc; call r001 1, , 3; say result
192: tc += 1; say "tc =" tc; call r001 1, , 3, ; say result
193: tc += 1; say "tc =" tc; call r001 1, , 3, 4; say result
194: tc += 1; say "tc =" tc; call r001 1, 2; say result
195: tc += 1; say "tc =" tc; call r001 1, 2, ; say result
196: tc += 1; say "tc =" tc; call r001 1, 2, , ; say result
197: tc += 1; say "tc =" tc; call r001 1, 2, , 4; say result
198: tc += 1; say "tc =" tc; call r001 1, 2, 3; say result
199: tc += 1; say "tc =" tc; call r001 1, 2, 3, ; say result
200: tc += 1; say "tc =" tc; call r001 1, 2, 3, 4; say result

-- r00d (labels 201: to 230:)
201: tc += 1; say "tc =" tc; call r00d; say result
202: tc += 1; say "tc =" tc; call r00d , ; say result
203: tc += 1; say "tc =" tc; call r00d , , ; say result
204: tc += 1; say "tc =" tc; call r00d , , , ; say result
205: tc += 1; say "tc =" tc; call r00d , , , 4; say result
206: tc += 1; say "tc =" tc; call r00d , , 3; say result
207: tc += 1; say "tc =" tc; call r00d , , 3, ; say result
208: tc += 1; say "tc =" tc; call r00d , , 3, 4; say result
209: tc += 1; say "tc =" tc; call r00d , 2; say result
210: tc += 1; say "tc =" tc; call r00d , 2, ; say result
211: tc += 1; say "tc =" tc; call r00d , 2, , ; say result
212: tc += 1; say "tc =" tc; call r00d , 2, , 4; say result
213: tc += 1; say "tc =" tc; call r00d , 2, 3; say result
214: tc += 1; say "tc =" tc; call r00d , 2, 3, ; say result
215: tc += 1; say "tc =" tc; call r00d , 2, 3, 4; say result
216: tc += 1; say "tc =" tc; call r00d 1; say result
217: tc += 1; say "tc =" tc; call r00d 1, ; say result
218: tc += 1; say "tc =" tc; call r00d 1, , ; say result
219: tc += 1; say "tc =" tc; call r00d 1, , , ; say result
220: tc += 1; say "tc =" tc; call r00d 1, , , 4; say result
221: tc += 1; say "tc =" tc; call r00d 1, , 3; say result
222: tc += 1; say "tc =" tc; call r00d 1, , 3, ; say result
223: tc += 1; say "tc =" tc; call r00d 1, , 3, 4; say result
224: tc += 1; say "tc =" tc; call r00d 1, 2; say result
225: tc += 1; say "tc =" tc; call r00d 1, 2, ; say result
226: tc += 1; say "tc =" tc; call r00d 1, 2, , ; say result
227: tc += 1; say "tc =" tc; call r00d 1, 2, , 4; say result
228: tc += 1; say "tc =" tc; call r00d 1, 2, 3; say result
229: tc += 1; say "tc =" tc; call r00d 1, 2, 3, ; say result
230: tc += 1; say "tc =" tc; call r00d 1, 2, 3, 4; say result

-- r010 (labels 231: to 260:)
231: tc += 1; say "tc =" tc; call r010; say result
232: tc += 1; say "tc =" tc; call r010 , ; say result
233: tc += 1; say "tc =" tc; call r010 , , ; say result
234: tc += 1; say "tc =" tc; call r010 , , , ; say result
235: tc += 1; say "tc =" tc; call r010 , , , 4; say result
236: tc += 1; say "tc =" tc; call r010 , , 3; say result
237: tc += 1; say "tc =" tc; call r010 , , 3, ; say result
238: tc += 1; say "tc =" tc; call r010 , , 3, 4; say result
239: tc += 1; say "tc =" tc; call r010 , 2; say result
240: tc += 1; say "tc =" tc; call r010 , 2, ; say result
241: tc += 1; say "tc =" tc; call r010 , 2, , ; say result
242: tc += 1; say "tc =" tc; call r010 , 2, , 4; say result
243: tc += 1; say "tc =" tc; call r010 , 2, 3; say result
244: tc += 1; say "tc =" tc; call r010 , 2, 3, ; say result
245: tc += 1; say "tc =" tc; call r010 , 2, 3, 4; say result
246: tc += 1; say "tc =" tc; call r010 1; say result
247: tc += 1; say "tc =" tc; call r010 1, ; say result
248: tc += 1; say "tc =" tc; call r010 1, , ; say result
249: tc += 1; say "tc =" tc; call r010 1, , , ; say result
250: tc += 1; say "tc =" tc; call r010 1, , , 4; say result
251: tc += 1; say "tc =" tc; call r010 1, , 3; say result
252: tc += 1; say "tc =" tc; call r010 1, , 3, ; say result
253: tc += 1; say "tc =" tc; call r010 1, , 3, 4; say result
254: tc += 1; say "tc =" tc; call r010 1, 2; say result
255: tc += 1; say "tc =" tc; call r010 1, 2, ; say result
256: tc += 1; say "tc =" tc; call r010 1, 2, , ; say result
257: tc += 1; say "tc =" tc; call r010 1, 2, , 4; say result
258: tc += 1; say "tc =" tc; call r010 1, 2, 3; say result
259: tc += 1; say "tc =" tc; call r010 1, 2, 3, ; say result
260: tc += 1; say "tc =" tc; call r010 1, 2, 3, 4; say result

-- r011 (labels 261: to 290:)
261: tc += 1; say "tc =" tc; call r011; say result
262: tc += 1; say "tc =" tc; call r011 , ; say result
263: tc += 1; say "tc =" tc; call r011 , , ; say result
264: tc += 1; say "tc =" tc; call r011 , , , ; say result
265: tc += 1; say "tc =" tc; call r011 , , , 4; say result
266: tc += 1; say "tc =" tc; call r011 , , 3; say result
267: tc += 1; say "tc =" tc; call r011 , , 3, ; say result
268: tc += 1; say "tc =" tc; call r011 , , 3, 4; say result
269: tc += 1; say "tc =" tc; call r011 , 2; say result
270: tc += 1; say "tc =" tc; call r011 , 2, ; say result
271: tc += 1; say "tc =" tc; call r011 , 2, , ; say result
272: tc += 1; say "tc =" tc; call r011 , 2, , 4; say result
273: tc += 1; say "tc =" tc; call r011 , 2, 3; say result
274: tc += 1; say "tc =" tc; call r011 , 2, 3, ; say result
275: tc += 1; say "tc =" tc; call r011 , 2, 3, 4; say result
276: tc += 1; say "tc =" tc; call r011 1; say result
277: tc += 1; say "tc =" tc; call r011 1, ; say result
278: tc += 1; say "tc =" tc; call r011 1, , ; say result
279: tc += 1; say "tc =" tc; call r011 1, , , ; say result
280: tc += 1; say "tc =" tc; call r011 1, , , 4; say result
281: tc += 1; say "tc =" tc; call r011 1, , 3; say result
282: tc += 1; say "tc =" tc; call r011 1, , 3, ; say result
283: tc += 1; say "tc =" tc; call r011 1, , 3, 4; say result
284: tc += 1; say "tc =" tc; call r011 1, 2; say result
285: tc += 1; say "tc =" tc; call r011 1, 2, ; say result
286: tc += 1; say "tc =" tc; call r011 1, 2, , ; say result
287: tc += 1; say "tc =" tc; call r011 1, 2, , 4; say result
288: tc += 1; say "tc =" tc; call r011 1, 2, 3; say result
289: tc += 1; say "tc =" tc; call r011 1, 2, 3, ; say result
290: tc += 1; say "tc =" tc; call r011 1, 2, 3, 4; say result

-- r01d (labels 291: to 320:)
291: tc += 1; say "tc =" tc; call r01d; say result
292: tc += 1; say "tc =" tc; call r01d , ; say result
293: tc += 1; say "tc =" tc; call r01d , , ; say result
294: tc += 1; say "tc =" tc; call r01d , , , ; say result
295: tc += 1; say "tc =" tc; call r01d , , , 4; say result
296: tc += 1; say "tc =" tc; call r01d , , 3; say result
297: tc += 1; say "tc =" tc; call r01d , , 3, ; say result
298: tc += 1; say "tc =" tc; call r01d , , 3, 4; say result
299: tc += 1; say "tc =" tc; call r01d , 2; say result
300: tc += 1; say "tc =" tc; call r01d , 2, ; say result
301: tc += 1; say "tc =" tc; call r01d , 2, , ; say result
302: tc += 1; say "tc =" tc; call r01d , 2, , 4; say result
303: tc += 1; say "tc =" tc; call r01d , 2, 3; say result
304: tc += 1; say "tc =" tc; call r01d , 2, 3, ; say result
305: tc += 1; say "tc =" tc; call r01d , 2, 3, 4; say result
306: tc += 1; say "tc =" tc; call r01d 1; say result
307: tc += 1; say "tc =" tc; call r01d 1, ; say result
308: tc += 1; say "tc =" tc; call r01d 1, , ; say result
309: tc += 1; say "tc =" tc; call r01d 1, , , ; say result
310: tc += 1; say "tc =" tc; call r01d 1, , , 4; say result
311: tc += 1; say "tc =" tc; call r01d 1, , 3; say result
312: tc += 1; say "tc =" tc; call r01d 1, , 3, ; say result
313: tc += 1; say "tc =" tc; call r01d 1, , 3, 4; say result
314: tc += 1; say "tc =" tc; call r01d 1, 2; say result
315: tc += 1; say "tc =" tc; call r01d 1, 2, ; say result
316: tc += 1; say "tc =" tc; call r01d 1, 2, , ; say result
317: tc += 1; say "tc =" tc; call r01d 1, 2, , 4; say result
318: tc += 1; say "tc =" tc; call r01d 1, 2, 3; say result
319: tc += 1; say "tc =" tc; call r01d 1, 2, 3, ; say result
320: tc += 1; say "tc =" tc; call r01d 1, 2, 3, 4; say result

-- r0d0 (labels 321: to 350:)
321: tc += 1; say "tc =" tc; call r0d0; say result
322: tc += 1; say "tc =" tc; call r0d0 , ; say result
323: tc += 1; say "tc =" tc; call r0d0 , , ; say result
324: tc += 1; say "tc =" tc; call r0d0 , , , ; say result
325: tc += 1; say "tc =" tc; call r0d0 , , , 4; say result
326: tc += 1; say "tc =" tc; call r0d0 , , 3; say result
327: tc += 1; say "tc =" tc; call r0d0 , , 3, ; say result
328: tc += 1; say "tc =" tc; call r0d0 , , 3, 4; say result
329: tc += 1; say "tc =" tc; call r0d0 , 2; say result
330: tc += 1; say "tc =" tc; call r0d0 , 2, ; say result
331: tc += 1; say "tc =" tc; call r0d0 , 2, , ; say result
332: tc += 1; say "tc =" tc; call r0d0 , 2, , 4; say result
333: tc += 1; say "tc =" tc; call r0d0 , 2, 3; say result
334: tc += 1; say "tc =" tc; call r0d0 , 2, 3, ; say result
335: tc += 1; say "tc =" tc; call r0d0 , 2, 3, 4; say result
336: tc += 1; say "tc =" tc; call r0d0 1; say result
337: tc += 1; say "tc =" tc; call r0d0 1, ; say result
338: tc += 1; say "tc =" tc; call r0d0 1, , ; say result
339: tc += 1; say "tc =" tc; call r0d0 1, , , ; say result
340: tc += 1; say "tc =" tc; call r0d0 1, , , 4; say result
341: tc += 1; say "tc =" tc; call r0d0 1, , 3; say result
342: tc += 1; say "tc =" tc; call r0d0 1, , 3, ; say result
343: tc += 1; say "tc =" tc; call r0d0 1, , 3, 4; say result
344: tc += 1; say "tc =" tc; call r0d0 1, 2; say result
345: tc += 1; say "tc =" tc; call r0d0 1, 2, ; say result
346: tc += 1; say "tc =" tc; call r0d0 1, 2, , ; say result
347: tc += 1; say "tc =" tc; call r0d0 1, 2, , 4; say result
348: tc += 1; say "tc =" tc; call r0d0 1, 2, 3; say result
349: tc += 1; say "tc =" tc; call r0d0 1, 2, 3, ; say result
350: tc += 1; say "tc =" tc; call r0d0 1, 2, 3, 4; say result

-- r0d1 (labels 351: to 380:)
351: tc += 1; say "tc =" tc; call r0d1; say result
352: tc += 1; say "tc =" tc; call r0d1 , ; say result
353: tc += 1; say "tc =" tc; call r0d1 , , ; say result
354: tc += 1; say "tc =" tc; call r0d1 , , , ; say result
355: tc += 1; say "tc =" tc; call r0d1 , , , 4; say result
356: tc += 1; say "tc =" tc; call r0d1 , , 3; say result
357: tc += 1; say "tc =" tc; call r0d1 , , 3, ; say result
358: tc += 1; say "tc =" tc; call r0d1 , , 3, 4; say result
359: tc += 1; say "tc =" tc; call r0d1 , 2; say result
360: tc += 1; say "tc =" tc; call r0d1 , 2, ; say result
361: tc += 1; say "tc =" tc; call r0d1 , 2, , ; say result
362: tc += 1; say "tc =" tc; call r0d1 , 2, , 4; say result
363: tc += 1; say "tc =" tc; call r0d1 , 2, 3; say result
364: tc += 1; say "tc =" tc; call r0d1 , 2, 3, ; say result
365: tc += 1; say "tc =" tc; call r0d1 , 2, 3, 4; say result
366: tc += 1; say "tc =" tc; call r0d1 1; say result
367: tc += 1; say "tc =" tc; call r0d1 1, ; say result
368: tc += 1; say "tc =" tc; call r0d1 1, , ; say result
369: tc += 1; say "tc =" tc; call r0d1 1, , , ; say result
370: tc += 1; say "tc =" tc; call r0d1 1, , , 4; say result
371: tc += 1; say "tc =" tc; call r0d1 1, , 3; say result
372: tc += 1; say "tc =" tc; call r0d1 1, , 3, ; say result
373: tc += 1; say "tc =" tc; call r0d1 1, , 3, 4; say result
374: tc += 1; say "tc =" tc; call r0d1 1, 2; say result
375: tc += 1; say "tc =" tc; call r0d1 1, 2, ; say result
376: tc += 1; say "tc =" tc; call r0d1 1, 2, , ; say result
377: tc += 1; say "tc =" tc; call r0d1 1, 2, , 4; say result
378: tc += 1; say "tc =" tc; call r0d1 1, 2, 3; say result
379: tc += 1; say "tc =" tc; call r0d1 1, 2, 3, ; say result
380: tc += 1; say "tc =" tc; call r0d1 1, 2, 3, 4; say result

-- r0dd (labels 381: to 410:)
381: tc += 1; say "tc =" tc; call r0dd; say result
382: tc += 1; say "tc =" tc; call r0dd , ; say result
383: tc += 1; say "tc =" tc; call r0dd , , ; say result
384: tc += 1; say "tc =" tc; call r0dd , , , ; say result
385: tc += 1; say "tc =" tc; call r0dd , , , 4; say result
386: tc += 1; say "tc =" tc; call r0dd , , 3; say result
387: tc += 1; say "tc =" tc; call r0dd , , 3, ; say result
388: tc += 1; say "tc =" tc; call r0dd , , 3, 4; say result
389: tc += 1; say "tc =" tc; call r0dd , 2; say result
390: tc += 1; say "tc =" tc; call r0dd , 2, ; say result
391: tc += 1; say "tc =" tc; call r0dd , 2, , ; say result
392: tc += 1; say "tc =" tc; call r0dd , 2, , 4; say result
393: tc += 1; say "tc =" tc; call r0dd , 2, 3; say result
394: tc += 1; say "tc =" tc; call r0dd , 2, 3, ; say result
395: tc += 1; say "tc =" tc; call r0dd , 2, 3, 4; say result
396: tc += 1; say "tc =" tc; call r0dd 1; say result
397: tc += 1; say "tc =" tc; call r0dd 1, ; say result
398: tc += 1; say "tc =" tc; call r0dd 1, , ; say result
399: tc += 1; say "tc =" tc; call r0dd 1, , , ; say result
400: tc += 1; say "tc =" tc; call r0dd 1, , , 4; say result
401: tc += 1; say "tc =" tc; call r0dd 1, , 3; say result
402: tc += 1; say "tc =" tc; call r0dd 1, , 3, ; say result
403: tc += 1; say "tc =" tc; call r0dd 1, , 3, 4; say result
404: tc += 1; say "tc =" tc; call r0dd 1, 2; say result
405: tc += 1; say "tc =" tc; call r0dd 1, 2, ; say result
406: tc += 1; say "tc =" tc; call r0dd 1, 2, , ; say result
407: tc += 1; say "tc =" tc; call r0dd 1, 2, , 4; say result
408: tc += 1; say "tc =" tc; call r0dd 1, 2, 3; say result
409: tc += 1; say "tc =" tc; call r0dd 1, 2, 3, ; say result
410: tc += 1; say "tc =" tc; call r0dd 1, 2, 3, 4; say result

-- r100 (labels 411: to 440:)
411: tc += 1; say "tc =" tc; call r100; say result
412: tc += 1; say "tc =" tc; call r100 , ; say result
413: tc += 1; say "tc =" tc; call r100 , , ; say result
414: tc += 1; say "tc =" tc; call r100 , , , ; say result
415: tc += 1; say "tc =" tc; call r100 , , , 4; say result
416: tc += 1; say "tc =" tc; call r100 , , 3; say result
417: tc += 1; say "tc =" tc; call r100 , , 3, ; say result
418: tc += 1; say "tc =" tc; call r100 , , 3, 4; say result
419: tc += 1; say "tc =" tc; call r100 , 2; say result
420: tc += 1; say "tc =" tc; call r100 , 2, ; say result
421: tc += 1; say "tc =" tc; call r100 , 2, , ; say result
422: tc += 1; say "tc =" tc; call r100 , 2, , 4; say result
423: tc += 1; say "tc =" tc; call r100 , 2, 3; say result
424: tc += 1; say "tc =" tc; call r100 , 2, 3, ; say result
425: tc += 1; say "tc =" tc; call r100 , 2, 3, 4; say result
426: tc += 1; say "tc =" tc; call r100 1; say result
427: tc += 1; say "tc =" tc; call r100 1, ; say result
428: tc += 1; say "tc =" tc; call r100 1, , ; say result
429: tc += 1; say "tc =" tc; call r100 1, , , ; say result
430: tc += 1; say "tc =" tc; call r100 1, , , 4; say result
431: tc += 1; say "tc =" tc; call r100 1, , 3; say result
432: tc += 1; say "tc =" tc; call r100 1, , 3, ; say result
433: tc += 1; say "tc =" tc; call r100 1, , 3, 4; say result
434: tc += 1; say "tc =" tc; call r100 1, 2; say result
435: tc += 1; say "tc =" tc; call r100 1, 2, ; say result
436: tc += 1; say "tc =" tc; call r100 1, 2, , ; say result
437: tc += 1; say "tc =" tc; call r100 1, 2, , 4; say result
438: tc += 1; say "tc =" tc; call r100 1, 2, 3; say result
439: tc += 1; say "tc =" tc; call r100 1, 2, 3, ; say result
440: tc += 1; say "tc =" tc; call r100 1, 2, 3, 4; say result

-- r101 (labels 441: to 470:)
441: tc += 1; say "tc =" tc; call r101; say result
442: tc += 1; say "tc =" tc; call r101 , ; say result
443: tc += 1; say "tc =" tc; call r101 , , ; say result
444: tc += 1; say "tc =" tc; call r101 , , , ; say result
445: tc += 1; say "tc =" tc; call r101 , , , 4; say result
446: tc += 1; say "tc =" tc; call r101 , , 3; say result
447: tc += 1; say "tc =" tc; call r101 , , 3, ; say result
448: tc += 1; say "tc =" tc; call r101 , , 3, 4; say result
449: tc += 1; say "tc =" tc; call r101 , 2; say result
450: tc += 1; say "tc =" tc; call r101 , 2, ; say result
451: tc += 1; say "tc =" tc; call r101 , 2, , ; say result
452: tc += 1; say "tc =" tc; call r101 , 2, , 4; say result
453: tc += 1; say "tc =" tc; call r101 , 2, 3; say result
454: tc += 1; say "tc =" tc; call r101 , 2, 3, ; say result
455: tc += 1; say "tc =" tc; call r101 , 2, 3, 4; say result
456: tc += 1; say "tc =" tc; call r101 1; say result
457: tc += 1; say "tc =" tc; call r101 1, ; say result
458: tc += 1; say "tc =" tc; call r101 1, , ; say result
459: tc += 1; say "tc =" tc; call r101 1, , , ; say result
460: tc += 1; say "tc =" tc; call r101 1, , , 4; say result
461: tc += 1; say "tc =" tc; call r101 1, , 3; say result
462: tc += 1; say "tc =" tc; call r101 1, , 3, ; say result
463: tc += 1; say "tc =" tc; call r101 1, , 3, 4; say result
464: tc += 1; say "tc =" tc; call r101 1, 2; say result
465: tc += 1; say "tc =" tc; call r101 1, 2, ; say result
466: tc += 1; say "tc =" tc; call r101 1, 2, , ; say result
467: tc += 1; say "tc =" tc; call r101 1, 2, , 4; say result
468: tc += 1; say "tc =" tc; call r101 1, 2, 3; say result
469: tc += 1; say "tc =" tc; call r101 1, 2, 3, ; say result
470: tc += 1; say "tc =" tc; call r101 1, 2, 3, 4; say result

-- r10d (labels 471: to 500:)
471: tc += 1; say "tc =" tc; call r10d; say result
472: tc += 1; say "tc =" tc; call r10d , ; say result
473: tc += 1; say "tc =" tc; call r10d , , ; say result
474: tc += 1; say "tc =" tc; call r10d , , , ; say result
475: tc += 1; say "tc =" tc; call r10d , , , 4; say result
476: tc += 1; say "tc =" tc; call r10d , , 3; say result
477: tc += 1; say "tc =" tc; call r10d , , 3, ; say result
478: tc += 1; say "tc =" tc; call r10d , , 3, 4; say result
479: tc += 1; say "tc =" tc; call r10d , 2; say result
480: tc += 1; say "tc =" tc; call r10d , 2, ; say result
481: tc += 1; say "tc =" tc; call r10d , 2, , ; say result
482: tc += 1; say "tc =" tc; call r10d , 2, , 4; say result
483: tc += 1; say "tc =" tc; call r10d , 2, 3; say result
484: tc += 1; say "tc =" tc; call r10d , 2, 3, ; say result
485: tc += 1; say "tc =" tc; call r10d , 2, 3, 4; say result
486: tc += 1; say "tc =" tc; call r10d 1; say result
487: tc += 1; say "tc =" tc; call r10d 1, ; say result
488: tc += 1; say "tc =" tc; call r10d 1, , ; say result
489: tc += 1; say "tc =" tc; call r10d 1, , , ; say result
490: tc += 1; say "tc =" tc; call r10d 1, , , 4; say result
491: tc += 1; say "tc =" tc; call r10d 1, , 3; say result
492: tc += 1; say "tc =" tc; call r10d 1, , 3, ; say result
493: tc += 1; say "tc =" tc; call r10d 1, , 3, 4; say result
494: tc += 1; say "tc =" tc; call r10d 1, 2; say result
495: tc += 1; say "tc =" tc; call r10d 1, 2, ; say result
496: tc += 1; say "tc =" tc; call r10d 1, 2, , ; say result
497: tc += 1; say "tc =" tc; call r10d 1, 2, , 4; say result
498: tc += 1; say "tc =" tc; call r10d 1, 2, 3; say result
499: tc += 1; say "tc =" tc; call r10d 1, 2, 3, ; say result
500: tc += 1; say "tc =" tc; call r10d 1, 2, 3, 4; say result

-- r110 (labels 501: to 530:)
501: tc += 1; say "tc =" tc; call r110; say result
502: tc += 1; say "tc =" tc; call r110 , ; say result
503: tc += 1; say "tc =" tc; call r110 , , ; say result
504: tc += 1; say "tc =" tc; call r110 , , , ; say result
505: tc += 1; say "tc =" tc; call r110 , , , 4; say result
506: tc += 1; say "tc =" tc; call r110 , , 3; say result
507: tc += 1; say "tc =" tc; call r110 , , 3, ; say result
508: tc += 1; say "tc =" tc; call r110 , , 3, 4; say result
509: tc += 1; say "tc =" tc; call r110 , 2; say result
510: tc += 1; say "tc =" tc; call r110 , 2, ; say result
511: tc += 1; say "tc =" tc; call r110 , 2, , ; say result
512: tc += 1; say "tc =" tc; call r110 , 2, , 4; say result
513: tc += 1; say "tc =" tc; call r110 , 2, 3; say result
514: tc += 1; say "tc =" tc; call r110 , 2, 3, ; say result
515: tc += 1; say "tc =" tc; call r110 , 2, 3, 4; say result
516: tc += 1; say "tc =" tc; call r110 1; say result
517: tc += 1; say "tc =" tc; call r110 1, ; say result
518: tc += 1; say "tc =" tc; call r110 1, , ; say result
519: tc += 1; say "tc =" tc; call r110 1, , , ; say result
520: tc += 1; say "tc =" tc; call r110 1, , , 4; say result
521: tc += 1; say "tc =" tc; call r110 1, , 3; say result
522: tc += 1; say "tc =" tc; call r110 1, , 3, ; say result
523: tc += 1; say "tc =" tc; call r110 1, , 3, 4; say result
524: tc += 1; say "tc =" tc; call r110 1, 2; say result
525: tc += 1; say "tc =" tc; call r110 1, 2, ; say result
526: tc += 1; say "tc =" tc; call r110 1, 2, , ; say result
527: tc += 1; say "tc =" tc; call r110 1, 2, , 4; say result
528: tc += 1; say "tc =" tc; call r110 1, 2, 3; say result
529: tc += 1; say "tc =" tc; call r110 1, 2, 3, ; say result
530: tc += 1; say "tc =" tc; call r110 1, 2, 3, 4; say result

-- r111 (labels 531: to 560:)
531: tc += 1; say "tc =" tc; call r111; say result
532: tc += 1; say "tc =" tc; call r111 , ; say result
533: tc += 1; say "tc =" tc; call r111 , , ; say result
534: tc += 1; say "tc =" tc; call r111 , , , ; say result
535: tc += 1; say "tc =" tc; call r111 , , , 4; say result
536: tc += 1; say "tc =" tc; call r111 , , 3; say result
537: tc += 1; say "tc =" tc; call r111 , , 3, ; say result
538: tc += 1; say "tc =" tc; call r111 , , 3, 4; say result
539: tc += 1; say "tc =" tc; call r111 , 2; say result
540: tc += 1; say "tc =" tc; call r111 , 2, ; say result
541: tc += 1; say "tc =" tc; call r111 , 2, , ; say result
542: tc += 1; say "tc =" tc; call r111 , 2, , 4; say result
543: tc += 1; say "tc =" tc; call r111 , 2, 3; say result
544: tc += 1; say "tc =" tc; call r111 , 2, 3, ; say result
545: tc += 1; say "tc =" tc; call r111 , 2, 3, 4; say result
546: tc += 1; say "tc =" tc; call r111 1; say result
547: tc += 1; say "tc =" tc; call r111 1, ; say result
548: tc += 1; say "tc =" tc; call r111 1, , ; say result
549: tc += 1; say "tc =" tc; call r111 1, , , ; say result
550: tc += 1; say "tc =" tc; call r111 1, , , 4; say result
551: tc += 1; say "tc =" tc; call r111 1, , 3; say result
552: tc += 1; say "tc =" tc; call r111 1, , 3, ; say result
553: tc += 1; say "tc =" tc; call r111 1, , 3, 4; say result
554: tc += 1; say "tc =" tc; call r111 1, 2; say result
555: tc += 1; say "tc =" tc; call r111 1, 2, ; say result
556: tc += 1; say "tc =" tc; call r111 1, 2, , ; say result
557: tc += 1; say "tc =" tc; call r111 1, 2, , 4; say result
558: tc += 1; say "tc =" tc; call r111 1, 2, 3; say result
559: tc += 1; say "tc =" tc; call r111 1, 2, 3, ; say result
560: tc += 1; say "tc =" tc; call r111 1, 2, 3, 4; say result

-- r11d (labels 561: to 590:)
561: tc += 1; say "tc =" tc; call r11d; say result
562: tc += 1; say "tc =" tc; call r11d , ; say result
563: tc += 1; say "tc =" tc; call r11d , , ; say result
564: tc += 1; say "tc =" tc; call r11d , , , ; say result
565: tc += 1; say "tc =" tc; call r11d , , , 4; say result
566: tc += 1; say "tc =" tc; call r11d , , 3; say result
567: tc += 1; say "tc =" tc; call r11d , , 3, ; say result
568: tc += 1; say "tc =" tc; call r11d , , 3, 4; say result
569: tc += 1; say "tc =" tc; call r11d , 2; say result
570: tc += 1; say "tc =" tc; call r11d , 2, ; say result
571: tc += 1; say "tc =" tc; call r11d , 2, , ; say result
572: tc += 1; say "tc =" tc; call r11d , 2, , 4; say result
573: tc += 1; say "tc =" tc; call r11d , 2, 3; say result
574: tc += 1; say "tc =" tc; call r11d , 2, 3, ; say result
575: tc += 1; say "tc =" tc; call r11d , 2, 3, 4; say result
576: tc += 1; say "tc =" tc; call r11d 1; say result
577: tc += 1; say "tc =" tc; call r11d 1, ; say result
578: tc += 1; say "tc =" tc; call r11d 1, , ; say result
579: tc += 1; say "tc =" tc; call r11d 1, , , ; say result
580: tc += 1; say "tc =" tc; call r11d 1, , , 4; say result
581: tc += 1; say "tc =" tc; call r11d 1, , 3; say result
582: tc += 1; say "tc =" tc; call r11d 1, , 3, ; say result
583: tc += 1; say "tc =" tc; call r11d 1, , 3, 4; say result
584: tc += 1; say "tc =" tc; call r11d 1, 2; say result
585: tc += 1; say "tc =" tc; call r11d 1, 2, ; say result
586: tc += 1; say "tc =" tc; call r11d 1, 2, , ; say result
587: tc += 1; say "tc =" tc; call r11d 1, 2, , 4; say result
588: tc += 1; say "tc =" tc; call r11d 1, 2, 3; say result
589: tc += 1; say "tc =" tc; call r11d 1, 2, 3, ; say result
590: tc += 1; say "tc =" tc; call r11d 1, 2, 3, 4; say result

-- r1d0 (labels 591: to 620:)
591: tc += 1; say "tc =" tc; call r1d0; say result
592: tc += 1; say "tc =" tc; call r1d0 , ; say result
593: tc += 1; say "tc =" tc; call r1d0 , , ; say result
594: tc += 1; say "tc =" tc; call r1d0 , , , ; say result
595: tc += 1; say "tc =" tc; call r1d0 , , , 4; say result
596: tc += 1; say "tc =" tc; call r1d0 , , 3; say result
597: tc += 1; say "tc =" tc; call r1d0 , , 3, ; say result
598: tc += 1; say "tc =" tc; call r1d0 , , 3, 4; say result
599: tc += 1; say "tc =" tc; call r1d0 , 2; say result
600: tc += 1; say "tc =" tc; call r1d0 , 2, ; say result
601: tc += 1; say "tc =" tc; call r1d0 , 2, , ; say result
602: tc += 1; say "tc =" tc; call r1d0 , 2, , 4; say result
603: tc += 1; say "tc =" tc; call r1d0 , 2, 3; say result
604: tc += 1; say "tc =" tc; call r1d0 , 2, 3, ; say result
605: tc += 1; say "tc =" tc; call r1d0 , 2, 3, 4; say result
606: tc += 1; say "tc =" tc; call r1d0 1; say result
607: tc += 1; say "tc =" tc; call r1d0 1, ; say result
608: tc += 1; say "tc =" tc; call r1d0 1, , ; say result
609: tc += 1; say "tc =" tc; call r1d0 1, , , ; say result
610: tc += 1; say "tc =" tc; call r1d0 1, , , 4; say result
611: tc += 1; say "tc =" tc; call r1d0 1, , 3; say result
612: tc += 1; say "tc =" tc; call r1d0 1, , 3, ; say result
613: tc += 1; say "tc =" tc; call r1d0 1, , 3, 4; say result
614: tc += 1; say "tc =" tc; call r1d0 1, 2; say result
615: tc += 1; say "tc =" tc; call r1d0 1, 2, ; say result
616: tc += 1; say "tc =" tc; call r1d0 1, 2, , ; say result
617: tc += 1; say "tc =" tc; call r1d0 1, 2, , 4; say result
618: tc += 1; say "tc =" tc; call r1d0 1, 2, 3; say result
619: tc += 1; say "tc =" tc; call r1d0 1, 2, 3, ; say result
620: tc += 1; say "tc =" tc; call r1d0 1, 2, 3, 4; say result

-- r1d1 (labels 621: to 650:)
621: tc += 1; say "tc =" tc; call r1d1; say result
622: tc += 1; say "tc =" tc; call r1d1 , ; say result
623: tc += 1; say "tc =" tc; call r1d1 , , ; say result
624: tc += 1; say "tc =" tc; call r1d1 , , , ; say result
625: tc += 1; say "tc =" tc; call r1d1 , , , 4; say result
626: tc += 1; say "tc =" tc; call r1d1 , , 3; say result
627: tc += 1; say "tc =" tc; call r1d1 , , 3, ; say result
628: tc += 1; say "tc =" tc; call r1d1 , , 3, 4; say result
629: tc += 1; say "tc =" tc; call r1d1 , 2; say result
630: tc += 1; say "tc =" tc; call r1d1 , 2, ; say result
631: tc += 1; say "tc =" tc; call r1d1 , 2, , ; say result
632: tc += 1; say "tc =" tc; call r1d1 , 2, , 4; say result
633: tc += 1; say "tc =" tc; call r1d1 , 2, 3; say result
634: tc += 1; say "tc =" tc; call r1d1 , 2, 3, ; say result
635: tc += 1; say "tc =" tc; call r1d1 , 2, 3, 4; say result
636: tc += 1; say "tc =" tc; call r1d1 1; say result
637: tc += 1; say "tc =" tc; call r1d1 1, ; say result
638: tc += 1; say "tc =" tc; call r1d1 1, , ; say result
639: tc += 1; say "tc =" tc; call r1d1 1, , , ; say result
640: tc += 1; say "tc =" tc; call r1d1 1, , , 4; say result
641: tc += 1; say "tc =" tc; call r1d1 1, , 3; say result
642: tc += 1; say "tc =" tc; call r1d1 1, , 3, ; say result
643: tc += 1; say "tc =" tc; call r1d1 1, , 3, 4; say result
644: tc += 1; say "tc =" tc; call r1d1 1, 2; say result
645: tc += 1; say "tc =" tc; call r1d1 1, 2, ; say result
646: tc += 1; say "tc =" tc; call r1d1 1, 2, , ; say result
647: tc += 1; say "tc =" tc; call r1d1 1, 2, , 4; say result
648: tc += 1; say "tc =" tc; call r1d1 1, 2, 3; say result
649: tc += 1; say "tc =" tc; call r1d1 1, 2, 3, ; say result
650: tc += 1; say "tc =" tc; call r1d1 1, 2, 3, 4; say result

-- r1dd (labels 651: to 680:)
651: tc += 1; say "tc =" tc; call r1dd; say result
652: tc += 1; say "tc =" tc; call r1dd , ; say result
653: tc += 1; say "tc =" tc; call r1dd , , ; say result
654: tc += 1; say "tc =" tc; call r1dd , , , ; say result
655: tc += 1; say "tc =" tc; call r1dd , , , 4; say result
656: tc += 1; say "tc =" tc; call r1dd , , 3; say result
657: tc += 1; say "tc =" tc; call r1dd , , 3, ; say result
658: tc += 1; say "tc =" tc; call r1dd , , 3, 4; say result
659: tc += 1; say "tc =" tc; call r1dd , 2; say result
660: tc += 1; say "tc =" tc; call r1dd , 2, ; say result
661: tc += 1; say "tc =" tc; call r1dd , 2, , ; say result
662: tc += 1; say "tc =" tc; call r1dd , 2, , 4; say result
663: tc += 1; say "tc =" tc; call r1dd , 2, 3; say result
664: tc += 1; say "tc =" tc; call r1dd , 2, 3, ; say result
665: tc += 1; say "tc =" tc; call r1dd , 2, 3, 4; say result
666: tc += 1; say "tc =" tc; call r1dd 1; say result
667: tc += 1; say "tc =" tc; call r1dd 1, ; say result
668: tc += 1; say "tc =" tc; call r1dd 1, , ; say result
669: tc += 1; say "tc =" tc; call r1dd 1, , , ; say result
670: tc += 1; say "tc =" tc; call r1dd 1, , , 4; say result
671: tc += 1; say "tc =" tc; call r1dd 1, , 3; say result
672: tc += 1; say "tc =" tc; call r1dd 1, , 3, ; say result
673: tc += 1; say "tc =" tc; call r1dd 1, , 3, 4; say result
674: tc += 1; say "tc =" tc; call r1dd 1, 2; say result
675: tc += 1; say "tc =" tc; call r1dd 1, 2, ; say result
676: tc += 1; say "tc =" tc; call r1dd 1, 2, , ; say result
677: tc += 1; say "tc =" tc; call r1dd 1, 2, , 4; say result
678: tc += 1; say "tc =" tc; call r1dd 1, 2, 3; say result
679: tc += 1; say "tc =" tc; call r1dd 1, 2, 3, ; say result
680: tc += 1; say "tc =" tc; call r1dd 1, 2, 3, 4; say result

-- rd00 (labels 681: to 710:)
681: tc += 1; say "tc =" tc; call rd00; say result
682: tc += 1; say "tc =" tc; call rd00 , ; say result
683: tc += 1; say "tc =" tc; call rd00 , , ; say result
684: tc += 1; say "tc =" tc; call rd00 , , , ; say result
685: tc += 1; say "tc =" tc; call rd00 , , , 4; say result
686: tc += 1; say "tc =" tc; call rd00 , , 3; say result
687: tc += 1; say "tc =" tc; call rd00 , , 3, ; say result
688: tc += 1; say "tc =" tc; call rd00 , , 3, 4; say result
689: tc += 1; say "tc =" tc; call rd00 , 2; say result
690: tc += 1; say "tc =" tc; call rd00 , 2, ; say result
691: tc += 1; say "tc =" tc; call rd00 , 2, , ; say result
692: tc += 1; say "tc =" tc; call rd00 , 2, , 4; say result
693: tc += 1; say "tc =" tc; call rd00 , 2, 3; say result
694: tc += 1; say "tc =" tc; call rd00 , 2, 3, ; say result
695: tc += 1; say "tc =" tc; call rd00 , 2, 3, 4; say result
696: tc += 1; say "tc =" tc; call rd00 1; say result
697: tc += 1; say "tc =" tc; call rd00 1, ; say result
698: tc += 1; say "tc =" tc; call rd00 1, , ; say result
699: tc += 1; say "tc =" tc; call rd00 1, , , ; say result
700: tc += 1; say "tc =" tc; call rd00 1, , , 4; say result
701: tc += 1; say "tc =" tc; call rd00 1, , 3; say result
702: tc += 1; say "tc =" tc; call rd00 1, , 3, ; say result
703: tc += 1; say "tc =" tc; call rd00 1, , 3, 4; say result
704: tc += 1; say "tc =" tc; call rd00 1, 2; say result
705: tc += 1; say "tc =" tc; call rd00 1, 2, ; say result
706: tc += 1; say "tc =" tc; call rd00 1, 2, , ; say result
707: tc += 1; say "tc =" tc; call rd00 1, 2, , 4; say result
708: tc += 1; say "tc =" tc; call rd00 1, 2, 3; say result
709: tc += 1; say "tc =" tc; call rd00 1, 2, 3, ; say result
710: tc += 1; say "tc =" tc; call rd00 1, 2, 3, 4; say result

-- rd01 (labels 711: to 740:)
711: tc += 1; say "tc =" tc; call rd01; say result
712: tc += 1; say "tc =" tc; call rd01 , ; say result
713: tc += 1; say "tc =" tc; call rd01 , , ; say result
714: tc += 1; say "tc =" tc; call rd01 , , , ; say result
715: tc += 1; say "tc =" tc; call rd01 , , , 4; say result
716: tc += 1; say "tc =" tc; call rd01 , , 3; say result
717: tc += 1; say "tc =" tc; call rd01 , , 3, ; say result
718: tc += 1; say "tc =" tc; call rd01 , , 3, 4; say result
719: tc += 1; say "tc =" tc; call rd01 , 2; say result
720: tc += 1; say "tc =" tc; call rd01 , 2, ; say result
721: tc += 1; say "tc =" tc; call rd01 , 2, , ; say result
722: tc += 1; say "tc =" tc; call rd01 , 2, , 4; say result
723: tc += 1; say "tc =" tc; call rd01 , 2, 3; say result
724: tc += 1; say "tc =" tc; call rd01 , 2, 3, ; say result
725: tc += 1; say "tc =" tc; call rd01 , 2, 3, 4; say result
726: tc += 1; say "tc =" tc; call rd01 1; say result
727: tc += 1; say "tc =" tc; call rd01 1, ; say result
728: tc += 1; say "tc =" tc; call rd01 1, , ; say result
729: tc += 1; say "tc =" tc; call rd01 1, , , ; say result
730: tc += 1; say "tc =" tc; call rd01 1, , , 4; say result
731: tc += 1; say "tc =" tc; call rd01 1, , 3; say result
732: tc += 1; say "tc =" tc; call rd01 1, , 3, ; say result
733: tc += 1; say "tc =" tc; call rd01 1, , 3, 4; say result
734: tc += 1; say "tc =" tc; call rd01 1, 2; say result
735: tc += 1; say "tc =" tc; call rd01 1, 2, ; say result
736: tc += 1; say "tc =" tc; call rd01 1, 2, , ; say result
737: tc += 1; say "tc =" tc; call rd01 1, 2, , 4; say result
738: tc += 1; say "tc =" tc; call rd01 1, 2, 3; say result
739: tc += 1; say "tc =" tc; call rd01 1, 2, 3, ; say result
740: tc += 1; say "tc =" tc; call rd01 1, 2, 3, 4; say result

-- rd0d (labels 741: to 770:)
741: tc += 1; say "tc =" tc; call rd0d; say result
742: tc += 1; say "tc =" tc; call rd0d , ; say result
743: tc += 1; say "tc =" tc; call rd0d , , ; say result
744: tc += 1; say "tc =" tc; call rd0d , , , ; say result
745: tc += 1; say "tc =" tc; call rd0d , , , 4; say result
746: tc += 1; say "tc =" tc; call rd0d , , 3; say result
747: tc += 1; say "tc =" tc; call rd0d , , 3, ; say result
748: tc += 1; say "tc =" tc; call rd0d , , 3, 4; say result
749: tc += 1; say "tc =" tc; call rd0d , 2; say result
750: tc += 1; say "tc =" tc; call rd0d , 2, ; say result
751: tc += 1; say "tc =" tc; call rd0d , 2, , ; say result
752: tc += 1; say "tc =" tc; call rd0d , 2, , 4; say result
753: tc += 1; say "tc =" tc; call rd0d , 2, 3; say result
754: tc += 1; say "tc =" tc; call rd0d , 2, 3, ; say result
755: tc += 1; say "tc =" tc; call rd0d , 2, 3, 4; say result
756: tc += 1; say "tc =" tc; call rd0d 1; say result
757: tc += 1; say "tc =" tc; call rd0d 1, ; say result
758: tc += 1; say "tc =" tc; call rd0d 1, , ; say result
759: tc += 1; say "tc =" tc; call rd0d 1, , , ; say result
760: tc += 1; say "tc =" tc; call rd0d 1, , , 4; say result
761: tc += 1; say "tc =" tc; call rd0d 1, , 3; say result
762: tc += 1; say "tc =" tc; call rd0d 1, , 3, ; say result
763: tc += 1; say "tc =" tc; call rd0d 1, , 3, 4; say result
764: tc += 1; say "tc =" tc; call rd0d 1, 2; say result
765: tc += 1; say "tc =" tc; call rd0d 1, 2, ; say result
766: tc += 1; say "tc =" tc; call rd0d 1, 2, , ; say result
767: tc += 1; say "tc =" tc; call rd0d 1, 2, , 4; say result
768: tc += 1; say "tc =" tc; call rd0d 1, 2, 3; say result
769: tc += 1; say "tc =" tc; call rd0d 1, 2, 3, ; say result
770: tc += 1; say "tc =" tc; call rd0d 1, 2, 3, 4; say result

-- rd10 (labels 771: to 800:)
771: tc += 1; say "tc =" tc; call rd10; say result
772: tc += 1; say "tc =" tc; call rd10 , ; say result
773: tc += 1; say "tc =" tc; call rd10 , , ; say result
774: tc += 1; say "tc =" tc; call rd10 , , , ; say result
775: tc += 1; say "tc =" tc; call rd10 , , , 4; say result
776: tc += 1; say "tc =" tc; call rd10 , , 3; say result
777: tc += 1; say "tc =" tc; call rd10 , , 3, ; say result
778: tc += 1; say "tc =" tc; call rd10 , , 3, 4; say result
779: tc += 1; say "tc =" tc; call rd10 , 2; say result
780: tc += 1; say "tc =" tc; call rd10 , 2, ; say result
781: tc += 1; say "tc =" tc; call rd10 , 2, , ; say result
782: tc += 1; say "tc =" tc; call rd10 , 2, , 4; say result
783: tc += 1; say "tc =" tc; call rd10 , 2, 3; say result
784: tc += 1; say "tc =" tc; call rd10 , 2, 3, ; say result
785: tc += 1; say "tc =" tc; call rd10 , 2, 3, 4; say result
786: tc += 1; say "tc =" tc; call rd10 1; say result
787: tc += 1; say "tc =" tc; call rd10 1, ; say result
788: tc += 1; say "tc =" tc; call rd10 1, , ; say result
789: tc += 1; say "tc =" tc; call rd10 1, , , ; say result
790: tc += 1; say "tc =" tc; call rd10 1, , , 4; say result
791: tc += 1; say "tc =" tc; call rd10 1, , 3; say result
792: tc += 1; say "tc =" tc; call rd10 1, , 3, ; say result
793: tc += 1; say "tc =" tc; call rd10 1, , 3, 4; say result
794: tc += 1; say "tc =" tc; call rd10 1, 2; say result
795: tc += 1; say "tc =" tc; call rd10 1, 2, ; say result
796: tc += 1; say "tc =" tc; call rd10 1, 2, , ; say result
797: tc += 1; say "tc =" tc; call rd10 1, 2, , 4; say result
798: tc += 1; say "tc =" tc; call rd10 1, 2, 3; say result
799: tc += 1; say "tc =" tc; call rd10 1, 2, 3, ; say result
800: tc += 1; say "tc =" tc; call rd10 1, 2, 3, 4; say result

-- rd11 (labels 801: to 830:)
801: tc += 1; say "tc =" tc; call rd11; say result
802: tc += 1; say "tc =" tc; call rd11 , ; say result
803: tc += 1; say "tc =" tc; call rd11 , , ; say result
804: tc += 1; say "tc =" tc; call rd11 , , , ; say result
805: tc += 1; say "tc =" tc; call rd11 , , , 4; say result
806: tc += 1; say "tc =" tc; call rd11 , , 3; say result
807: tc += 1; say "tc =" tc; call rd11 , , 3, ; say result
808: tc += 1; say "tc =" tc; call rd11 , , 3, 4; say result
809: tc += 1; say "tc =" tc; call rd11 , 2; say result
810: tc += 1; say "tc =" tc; call rd11 , 2, ; say result
811: tc += 1; say "tc =" tc; call rd11 , 2, , ; say result
812: tc += 1; say "tc =" tc; call rd11 , 2, , 4; say result
813: tc += 1; say "tc =" tc; call rd11 , 2, 3; say result
814: tc += 1; say "tc =" tc; call rd11 , 2, 3, ; say result
815: tc += 1; say "tc =" tc; call rd11 , 2, 3, 4; say result
816: tc += 1; say "tc =" tc; call rd11 1; say result
817: tc += 1; say "tc =" tc; call rd11 1, ; say result
818: tc += 1; say "tc =" tc; call rd11 1, , ; say result
819: tc += 1; say "tc =" tc; call rd11 1, , , ; say result
820: tc += 1; say "tc =" tc; call rd11 1, , , 4; say result
821: tc += 1; say "tc =" tc; call rd11 1, , 3; say result
822: tc += 1; say "tc =" tc; call rd11 1, , 3, ; say result
823: tc += 1; say "tc =" tc; call rd11 1, , 3, 4; say result
824: tc += 1; say "tc =" tc; call rd11 1, 2; say result
825: tc += 1; say "tc =" tc; call rd11 1, 2, ; say result
826: tc += 1; say "tc =" tc; call rd11 1, 2, , ; say result
827: tc += 1; say "tc =" tc; call rd11 1, 2, , 4; say result
828: tc += 1; say "tc =" tc; call rd11 1, 2, 3; say result
829: tc += 1; say "tc =" tc; call rd11 1, 2, 3, ; say result
830: tc += 1; say "tc =" tc; call rd11 1, 2, 3, 4; say result

-- rd1d (labels 831: to 860:)
831: tc += 1; say "tc =" tc; call rd1d; say result
832: tc += 1; say "tc =" tc; call rd1d , ; say result
833: tc += 1; say "tc =" tc; call rd1d , , ; say result
834: tc += 1; say "tc =" tc; call rd1d , , , ; say result
835: tc += 1; say "tc =" tc; call rd1d , , , 4; say result
836: tc += 1; say "tc =" tc; call rd1d , , 3; say result
837: tc += 1; say "tc =" tc; call rd1d , , 3, ; say result
838: tc += 1; say "tc =" tc; call rd1d , , 3, 4; say result
839: tc += 1; say "tc =" tc; call rd1d , 2; say result
840: tc += 1; say "tc =" tc; call rd1d , 2, ; say result
841: tc += 1; say "tc =" tc; call rd1d , 2, , ; say result
842: tc += 1; say "tc =" tc; call rd1d , 2, , 4; say result
843: tc += 1; say "tc =" tc; call rd1d , 2, 3; say result
844: tc += 1; say "tc =" tc; call rd1d , 2, 3, ; say result
845: tc += 1; say "tc =" tc; call rd1d , 2, 3, 4; say result
846: tc += 1; say "tc =" tc; call rd1d 1; say result
847: tc += 1; say "tc =" tc; call rd1d 1, ; say result
848: tc += 1; say "tc =" tc; call rd1d 1, , ; say result
849: tc += 1; say "tc =" tc; call rd1d 1, , , ; say result
850: tc += 1; say "tc =" tc; call rd1d 1, , , 4; say result
851: tc += 1; say "tc =" tc; call rd1d 1, , 3; say result
852: tc += 1; say "tc =" tc; call rd1d 1, , 3, ; say result
853: tc += 1; say "tc =" tc; call rd1d 1, , 3, 4; say result
854: tc += 1; say "tc =" tc; call rd1d 1, 2; say result
855: tc += 1; say "tc =" tc; call rd1d 1, 2, ; say result
856: tc += 1; say "tc =" tc; call rd1d 1, 2, , ; say result
857: tc += 1; say "tc =" tc; call rd1d 1, 2, , 4; say result
858: tc += 1; say "tc =" tc; call rd1d 1, 2, 3; say result
859: tc += 1; say "tc =" tc; call rd1d 1, 2, 3, ; say result
860: tc += 1; say "tc =" tc; call rd1d 1, 2, 3, 4; say result

-- rdd0 (labels 861: to 890:)
861: tc += 1; say "tc =" tc; call rdd0; say result
862: tc += 1; say "tc =" tc; call rdd0 , ; say result
863: tc += 1; say "tc =" tc; call rdd0 , , ; say result
864: tc += 1; say "tc =" tc; call rdd0 , , , ; say result
865: tc += 1; say "tc =" tc; call rdd0 , , , 4; say result
866: tc += 1; say "tc =" tc; call rdd0 , , 3; say result
867: tc += 1; say "tc =" tc; call rdd0 , , 3, ; say result
868: tc += 1; say "tc =" tc; call rdd0 , , 3, 4; say result
869: tc += 1; say "tc =" tc; call rdd0 , 2; say result
870: tc += 1; say "tc =" tc; call rdd0 , 2, ; say result
871: tc += 1; say "tc =" tc; call rdd0 , 2, , ; say result
872: tc += 1; say "tc =" tc; call rdd0 , 2, , 4; say result
873: tc += 1; say "tc =" tc; call rdd0 , 2, 3; say result
874: tc += 1; say "tc =" tc; call rdd0 , 2, 3, ; say result
875: tc += 1; say "tc =" tc; call rdd0 , 2, 3, 4; say result
876: tc += 1; say "tc =" tc; call rdd0 1; say result
877: tc += 1; say "tc =" tc; call rdd0 1, ; say result
878: tc += 1; say "tc =" tc; call rdd0 1, , ; say result
879: tc += 1; say "tc =" tc; call rdd0 1, , , ; say result
880: tc += 1; say "tc =" tc; call rdd0 1, , , 4; say result
881: tc += 1; say "tc =" tc; call rdd0 1, , 3; say result
882: tc += 1; say "tc =" tc; call rdd0 1, , 3, ; say result
883: tc += 1; say "tc =" tc; call rdd0 1, , 3, 4; say result
884: tc += 1; say "tc =" tc; call rdd0 1, 2; say result
885: tc += 1; say "tc =" tc; call rdd0 1, 2, ; say result
886: tc += 1; say "tc =" tc; call rdd0 1, 2, , ; say result
887: tc += 1; say "tc =" tc; call rdd0 1, 2, , 4; say result
888: tc += 1; say "tc =" tc; call rdd0 1, 2, 3; say result
889: tc += 1; say "tc =" tc; call rdd0 1, 2, 3, ; say result
890: tc += 1; say "tc =" tc; call rdd0 1, 2, 3, 4; say result

-- rdd1 (labels 891: to 920:)
891: tc += 1; say "tc =" tc; call rdd1; say result
892: tc += 1; say "tc =" tc; call rdd1 , ; say result
893: tc += 1; say "tc =" tc; call rdd1 , , ; say result
894: tc += 1; say "tc =" tc; call rdd1 , , , ; say result
895: tc += 1; say "tc =" tc; call rdd1 , , , 4; say result
896: tc += 1; say "tc =" tc; call rdd1 , , 3; say result
897: tc += 1; say "tc =" tc; call rdd1 , , 3, ; say result
898: tc += 1; say "tc =" tc; call rdd1 , , 3, 4; say result
899: tc += 1; say "tc =" tc; call rdd1 , 2; say result
900: tc += 1; say "tc =" tc; call rdd1 , 2, ; say result
901: tc += 1; say "tc =" tc; call rdd1 , 2, , ; say result
902: tc += 1; say "tc =" tc; call rdd1 , 2, , 4; say result
903: tc += 1; say "tc =" tc; call rdd1 , 2, 3; say result
904: tc += 1; say "tc =" tc; call rdd1 , 2, 3, ; say result
905: tc += 1; say "tc =" tc; call rdd1 , 2, 3, 4; say result
906: tc += 1; say "tc =" tc; call rdd1 1; say result
907: tc += 1; say "tc =" tc; call rdd1 1, ; say result
908: tc += 1; say "tc =" tc; call rdd1 1, , ; say result
909: tc += 1; say "tc =" tc; call rdd1 1, , , ; say result
910: tc += 1; say "tc =" tc; call rdd1 1, , , 4; say result
911: tc += 1; say "tc =" tc; call rdd1 1, , 3; say result
912: tc += 1; say "tc =" tc; call rdd1 1, , 3, ; say result
913: tc += 1; say "tc =" tc; call rdd1 1, , 3, 4; say result
914: tc += 1; say "tc =" tc; call rdd1 1, 2; say result
915: tc += 1; say "tc =" tc; call rdd1 1, 2, ; say result
916: tc += 1; say "tc =" tc; call rdd1 1, 2, , ; say result
917: tc += 1; say "tc =" tc; call rdd1 1, 2, , 4; say result
918: tc += 1; say "tc =" tc; call rdd1 1, 2, 3; say result
919: tc += 1; say "tc =" tc; call rdd1 1, 2, 3, ; say result
920: tc += 1; say "tc =" tc; call rdd1 1, 2, 3, 4; say result

-- rddd (labels 921: to 950:)
921: tc += 1; say "tc =" tc; call rddd; say result
922: tc += 1; say "tc =" tc; call rddd , ; say result
923: tc += 1; say "tc =" tc; call rddd , , ; say result
924: tc += 1; say "tc =" tc; call rddd , , , ; say result
925: tc += 1; say "tc =" tc; call rddd , , , 4; say result
926: tc += 1; say "tc =" tc; call rddd , , 3; say result
927: tc += 1; say "tc =" tc; call rddd , , 3, ; say result
928: tc += 1; say "tc =" tc; call rddd , , 3, 4; say result
929: tc += 1; say "tc =" tc; call rddd , 2; say result
930: tc += 1; say "tc =" tc; call rddd , 2, ; say result
931: tc += 1; say "tc =" tc; call rddd , 2, , ; say result
932: tc += 1; say "tc =" tc; call rddd , 2, , 4; say result
933: tc += 1; say "tc =" tc; call rddd , 2, 3; say result
934: tc += 1; say "tc =" tc; call rddd , 2, 3, ; say result
935: tc += 1; say "tc =" tc; call rddd , 2, 3, 4; say result
936: tc += 1; say "tc =" tc; call rddd 1; say result
937: tc += 1; say "tc =" tc; call rddd 1, ; say result
938: tc += 1; say "tc =" tc; call rddd 1, , ; say result
939: tc += 1; say "tc =" tc; call rddd 1, , , ; say result
940: tc += 1; say "tc =" tc; call rddd 1, , , 4; say result
941: tc += 1; say "tc =" tc; call rddd 1, , 3; say result
942: tc += 1; say "tc =" tc; call rddd 1, , 3, ; say result
943: tc += 1; say "tc =" tc; call rddd 1, , 3, 4; say result
944: tc += 1; say "tc =" tc; call rddd 1, 2; say result
945: tc += 1; say "tc =" tc; call rddd 1, 2, ; say result
946: tc += 1; say "tc =" tc; call rddd 1, 2, , ; say result
947: tc += 1; say "tc =" tc; call rddd 1, 2, , 4; say result
948: tc += 1; say "tc =" tc; call rddd 1, 2, 3; say result
949: tc += 1; say "tc =" tc; call rddd 1, 2, 3, ; say result
950: tc += 1; say "tc =" tc; call rddd 1, 2, 3, 4; say result

951: exit

syntax:
    call sayCondition condition("O")
    signal on syntax
    signal value tc + 1

::routine sayCondition
    use strict arg condition
    if condition == .nil then return

    if condition~condition <> "SYNTAX" then .error~say(finalDot(condition~condition))
    if condition~description <> .nil, condition~description <> "" then .error~say(finalDot(condition~description))

    -- For SYNTAX conditions
    if condition~message <> .nil then .error~say(finalDot(condition~message))
    else if condition~errortext <> .nil then .error~say(finalDot(condition~errortext))
    if condition~code <> .nil then .error~say("Error code=" condition~code)

-- ooRexx 4.2 doesnt display a final dot.
-- ooRexx 5.2 displays a final dot.
-- Must harmonize for diff.
::routine finalDot
    use strict arg string
    if string~right(1) = "." then return string
    return string"."

::routine r0
    say arg(); use strict arg; return arg()

::routine r1
    say arg(); use strict arg a1; return arg()

::routine rd
    say arg(); use strict arg a1=1; return arg()

::routine r00
    say arg(); use strict arg , ; return arg()

::routine r01
    say arg(); use strict arg , a2; return arg()

::routine r0d
    say arg(); use strict arg , a2=2; return arg()

::routine r10
    say arg(); use strict arg a1, ; return arg()

::routine r11
    say arg(); use strict arg a1, a2; return arg()

::routine r1d
    say arg(); use strict arg a1, a2=2; return arg()

::routine rd0
    say arg(); use strict arg a1=1, ; return arg()

::routine rd1
    say arg(); use strict arg a1=1, a2; return arg()

::routine rdd
    say arg(); use strict arg a1=1, a2=1; return arg()

::routine r000
    say arg(); use strict arg , , ; return arg()

::routine r001
    say arg(); use strict arg , , a3; return arg()

::routine r00d
    say arg(); use strict arg , , a3=3; return arg()

::routine r010
    say arg(); use strict arg , a2, ; return arg()

::routine r011
    say arg(); use strict arg , a2, a3; return arg()

::routine r01d
    say arg(); use strict arg , a2, a3=3; return arg()

::routine r0d0
    say arg(); use strict arg , a2=2, ; return arg()

::routine r0d1
    say arg(); use strict arg , a2=2, a3; return arg()

::routine r0dd
    say arg(); use strict arg , a2=2, a3=3; return arg()

::routine r100
    say arg(); use strict arg a1, , ; return arg()

::routine r101
    say arg(); use strict arg a1, , a3; return arg()

::routine r10d
    say arg(); use strict arg a1, , a3=3; return arg()

::routine r110
    say arg(); use strict arg a1, a2, ; return arg()

::routine r111
    say arg(); use strict arg a1, a2, a3; return arg()

::routine r11d
    say arg(); use strict arg a1, a2, a3=3; return arg()

::routine r1d0
    say arg(); use strict arg a1, a2=2, ; return arg()

::routine r1d1
    say arg(); use strict arg a1, a2=2, a3; return arg()

::routine r1dd
    say arg(); use strict arg a1, a2=2, a3=3; return arg()

::routine rd00
    say arg(); use strict arg a1=1, , ; return arg()

::routine rd01
    say arg(); use strict arg a1=1, , a3; return arg()

::routine rd0d
    say arg(); use strict arg a1=1, , a3=3; return arg()

::routine rd10
    say arg(); use strict arg a1=1, a2, ; return arg()

::routine rd11
    say arg(); use strict arg a1=1, a2, a3; return arg()

::routine rd1d
    say arg(); use strict arg a1=1, a2, a3=3; return arg()

::routine rdd0
    say arg(); use strict arg a1=1, a2=2, ; return arg()

::routine rdd1
    say arg(); use strict arg a1=1, a2=2, a3; return arg()

::routine rddd
    say arg(); use strict arg a1=1, a2=2, a3=3; return arg()


/*
callee side
trinary: 0 1 d
    0   name not declared
    1   name declared
    d   name declared with default value

-- callee side: 1 arg
-- 3 combinations
0
1
d

-- callee side: 2 args
-- 3*3 combinations
00
01
0d
10
11
1d
d0
d1
dd

-- callee side: 3 args
-- 3*3*3 combinations
000
001
00d
010
011
01d
0d0
0d1
0dd
100
101
10d
110
111
11d
1d0
1d1
1dd
d00
d01
d0d
d10
d11
d1d
dd0
dd1
ddd
*/

/*
caller side: 2 args + 1 extra
    0: the argument is omitted
    1: the argument is passed

000
r
r ,
r , ,

001
r , , 3

010
r , 2
r , 2,

011
r , 2, 3

100
r 1
r 1,
r 1, ,

101
r 1, , 3

110
r 1, 2
r 1, 2,

111
r 1, 2, 3
*/

/*
caller side: 3 args + 1 extra
    0: the argument is omitted
    1: the argument is passed

0000
r
r ,
r , ,
r , , ,

0001
r , , , 4

0010
r , , 3
r , , 3,

0011
r , , 3, 4

0100
r , 2
r , 2,
r , 2, ,

0101
r , 2, , 4

0110
r , 2, 3
r , 2, 3,

0111
r , 2, 3, 4

1000
r 1
r 1,
r 1, ,
r 1, , ,

1001
r 1, , , 4

1010
r 1, , 3
r 1, , 3,

1011
r 1, , 3, 4

1100
r 1, 2
r 1, 2,
r 1, 2, ,

1101
r 1, 2, , 4

1110
r 1, 2, 3
r 1, 2, 3,

1111
r 1, 2, 3, 4
*/