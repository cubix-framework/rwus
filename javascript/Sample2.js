TestCoverage = {coverage: []}

function assertThat(predicate){
  if (!predicate){
    throw new Error("Assertion Failed");
  }
}

var correctFormats = [
	['dddd, MMMM Do YYYY, h:mm:ss a',      'Ø§ÙØ£Ø­Ø¯Ø Ø´Ø¨Ø§Ø· ÙØ¨Ø±Ø§ÙØ± Ù¡Ù¤ Ù¢Ù Ù¡Ù Ø Ù£:Ù¢Ù¥:Ù¥Ù  Ù'],
	['ddd, hA',                            'Ø£Ø­Ø¯Ø Ù£Ù'],
	['M Mo MM MMMM MMM',                   'Ù¢ Ù¢ Ù Ù¢ Ø´Ø¨Ø§Ø· ÙØ¨Ø±Ø§ÙØ± Ø´Ø¨Ø§Ø· ÙØ¨Ø±Ø§ÙØ±'],
	['YYYY YY',                            'Ù¢Ù Ù¡Ù  Ù¡Ù '],
	['D Do DD',                            'Ù¡Ù¤ Ù¡Ù¤ Ù¡Ù¤'],
	['d do dddd ddd dd',                   'Ù  Ù  Ø§ÙØ£Ø­Ø¯ Ø£Ø­Ø¯ Ø­'],
	['DDD DDDo DDDD',                      'Ù¤Ù¥ Ù¤Ù¥ Ù Ù¤Ù¥'],
	['w wo ww',                            'Ù¨ Ù¨ Ù Ù¨'],
	['h hh',                               'Ù£ Ù Ù£'],
	['H HH',                               'Ù¡Ù¥ Ù¡Ù¥'],
	['m mm',                               'Ù¢Ù¥ Ù¢Ù¥'],
	['s ss',                               'Ù¥Ù  Ù¥Ù '],
	['a A',                                'Ù Ù'],
	['[the] DDDo [day of the year]',       'the Ù¤Ù¥ day of the year'],
	['LT',                                 'Ù¡Ù¥:Ù¢Ù¥'],
	['LTS',                                'Ù¡Ù¥:Ù¢Ù¥:Ù¥Ù '],
	['L',                                  'Ù¡Ù¤/\u200fÙ¢/\u200fÙ¢Ù Ù¡Ù '],
	['LL',                                 'Ù¡Ù¤ Ø´Ø¨Ø§Ø· ÙØ¨Ø±Ø§ÙØ± Ù¢Ù Ù¡Ù '],
	['LLL',                                'Ù¡Ù¤ Ø´Ø¨Ø§Ø· ÙØ¨Ø±Ø§ÙØ± Ù¢Ù Ù¡Ù  Ù¡Ù¥:Ù¢Ù¥'],
	['LLLL',                               'Ø§ÙØ£Ø­Ø¯ Ù¡Ù¤ Ø´Ø¨Ø§Ø· ÙØ¨Ø±Ø§ÙØ± Ù¢Ù Ù¡Ù  Ù¡Ù¥:Ù¢Ù¥'],
	['l',                                  'Ù¡Ù¤/\u200fÙ¢/\u200fÙ¢Ù Ù¡Ù '],
	['ll',                                 'Ù¡Ù¤ Ø´Ø¨Ø§Ø· ÙØ¨Ø±Ø§ÙØ± Ù¢Ù Ù¡Ù '],
	['lll',                                'Ù¡Ù¤ Ø´Ø¨Ø§Ø· ÙØ¨Ø±Ø§ÙØ± Ù¢Ù Ù¡Ù  Ù¡Ù¥:Ù¢Ù¥'],
	['llll',                               'Ø£Ø­Ø¯ Ù¡Ù¤ Ø´Ø¨Ø§Ø· ÙØ¨Ø±Ø§ÙØ± Ù¢Ù Ù¡Ù  Ù¡Ù¥:Ù¢Ù¥']
]

var referenceFormats = [
	['dddd, MMMM Do YYYY, h:mm:ss a',      'format_output_1'],
	['ddd, hA',                            'format_output_2'],
	['M Mo MM MMMM MMM',                   'format_output_3'],
	['YYYY YY',                            'format_output_4'],
	['D Do DD',                            'format_output_5'],
	['d do dddd ddd dd',                   'format_output_6'],
	['DDD DDDo DDDD',                      'format_output_7'],
	['w wo ww',                            'format_output_8'],
	['h hh',                               'format_output_9'],
	['H HH',                               'format_output_10'],
	['m mm',                               'format_output_11'],
	['s ss',                               'format_output_12'],
	['a A',                                'format_output_13'],
	['[the] DDDo [day of the year]',       'format_output_14'],
	['LT',                                 'format_output_15'],
	['LTS',                                'format_output_16'],
	['L',                                  'format_output_17'],
	['LL',                                 'format_output_18'],
	['LLL',                                'format_output_19'],
	['LLLL',                               'format_output_20'],
	['l',                                  'format_output_21'],
	['ll',                                 'format_output_22'],
	['lll',                                'format_output_23'],
	['llll',                               'format_output_24']
]

var referenceFormatMap = {};

for (var i = 0; i < referenceFormats.length; i++) {
    referenceFormatMap[referenceFormats[i][0]] = referenceFormats[i][1];
}

var calls = []
function format(formatStr){
	calls.push(["format", [formatStr]])
	return referenceFormatMap[formatStr]
}

function moment(date){
	calls.push(["moment", [date]])
	assertThat(date.getTime() === new Date(2010, 1, 14, 15, 25, 50, 125).getTime())
	return {
		format: format
	}
}

function equal(v1, v2, debugStr){
	calls.push(["equal", [v1,v2,debugStr]])
}

var assertObj = {
	equal: equal
}

#START OF SAMPLE    
function testFormat(assert) {
	var a = [
		['dddd, MMMM Do YYYY, h:mm:ss a',      'Ø§ÙØ£Ø­Ø¯Ø Ø´Ø¨Ø§Ø· ÙØ¨Ø±Ø§ÙØ± Ù¡Ù¤ Ù¢Ù Ù¡Ù Ø Ù£:Ù¢Ù¥:Ù¥Ù  Ù'],
		['ddd, hA',                            'Ø£Ø­Ø¯Ø Ù£Ù'],
		['M Mo MM MMMM MMM',                   'Ù¢ Ù¢ Ù Ù¢ Ø´Ø¨Ø§Ø· ÙØ¨Ø±Ø§ÙØ± Ø´Ø¨Ø§Ø· ÙØ¨Ø±Ø§ÙØ±'],
		['YYYY YY',                            'Ù¢Ù Ù¡Ù  Ù¡Ù '],
		['D Do DD',                            'Ù¡Ù¤ Ù¡Ù¤ Ù¡Ù¤'],
		['d do dddd ddd dd',                   'Ù  Ù  Ø§ÙØ£Ø­Ø¯ Ø£Ø­Ø¯ Ø­'],
		['DDD DDDo DDDD',                      'Ù¤Ù¥ Ù¤Ù¥ Ù Ù¤Ù¥'],
		['w wo ww',                            'Ù¨ Ù¨ Ù Ù¨'],
		['h hh',                               'Ù£ Ù Ù£'],
		['H HH',                               'Ù¡Ù¥ Ù¡Ù¥'],
		['m mm',                               'Ù¢Ù¥ Ù¢Ù¥'],
		['s ss',                               'Ù¥Ù  Ù¥Ù '],
		['a A',                                'Ù Ù'],
		['[the] DDDo [day of the year]',       'the Ù¤Ù¥ day of the year'],
		['LT',                                 'Ù¡Ù¥:Ù¢Ù¥'],
		['LTS',                                'Ù¡Ù¥:Ù¢Ù¥:Ù¥Ù '],
		['L',                                  'Ù¡Ù¤/\u200fÙ¢/\u200fÙ¢Ù Ù¡Ù '],
		['LL',                                 'Ù¡Ù¤ Ø´Ø¨Ø§Ø· ÙØ¨Ø±Ø§ÙØ± Ù¢Ù Ù¡Ù '],
		['LLL',                                'Ù¡Ù¤ Ø´Ø¨Ø§Ø· ÙØ¨Ø±Ø§ÙØ± Ù¢Ù Ù¡Ù  Ù¡Ù¥:Ù¢Ù¥'],
		['LLLL',                               'Ø§ÙØ£Ø­Ø¯ Ù¡Ù¤ Ø´Ø¨Ø§Ø· ÙØ¨Ø±Ø§ÙØ± Ù¢Ù Ù¡Ù  Ù¡Ù¥:Ù¢Ù¥'],
		['l',                                  'Ù¡Ù¤/\u200fÙ¢/\u200fÙ¢Ù Ù¡Ù '],
		['ll',                                 'Ù¡Ù¤ Ø´Ø¨Ø§Ø· ÙØ¨Ø±Ø§ÙØ± Ù¢Ù Ù¡Ù '],
		['lll',                                'Ù¡Ù¤ Ø´Ø¨Ø§Ø· ÙØ¨Ø±Ø§ÙØ± Ù¢Ù Ù¡Ù  Ù¡Ù¥:Ù¢Ù¥'],
		['llll',                               'Ø£Ø­Ø¯ Ù¡Ù¤ Ø´Ø¨Ø§Ø· ÙØ¨Ø±Ø§ÙØ± Ù¢Ù Ù¡Ù  Ù¡Ù¥:Ù¢Ù¥']
	],
	b = moment(new Date(2010, 1, 14, 15, 25, 50, 125)),	i;
	for (i = 0; i < a.length; i++) {
		assert.equal(b.format(a[i][0]), a[i][1], a[i][0] + ' ---> ' + a[i][1]);
	}
}
#END OF SAMPLE

// tests

testFormat(assertObj)
assertThat(calls[0][0] === "moment")
for (var i = 0; i < 24; i++){
	var callIdx = 1+i*2
	assertThat(calls[callIdx][0] === "format")
		assertThat(calls[callIdx][1][0] === correctFormats[i][0])
	assertThat(calls[callIdx+1][0] === "equal")
		assertThat(calls[callIdx+1][1][0] === referenceFormats[i][1])
		assertThat(calls[callIdx+1][1][1] === correctFormats[i][1])
		assertThat(calls[callIdx+1][1][2] === correctFormats[i][0] + ' ---> ' + correctFormats[i][1])
}
assertThat(calls.length === 49)


