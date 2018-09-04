TestCoverage = {coverage: []}

function assert(predicate){
  if (!predicate){
    throw new Error("Assertion Failed");
  }
}

function nearlyEqual(a, b){
  var diff = Math.abs(Math.abs(a) - Math.abs(b))
  return diff < 0.000000000001
}


function Sample3(r, g, b){
  this.r = r
  this.g = g
  this.b = b

#START OF SAMPLE      
  this.getHSL = function(optionalTarget) {
    // h,s,l ranges are in 0.0 - 1.0

    var hsl = optionalTarget || { h: 0, s: 0, l: 0 };

    var r = this.r, g = this.g, b = this.b;

    var max = Math.max( r, g, b );
    var min = Math.min( r, g, b );

    var hue, saturation;
    var lightness = ( min + max ) / 2.0;

    if ( min === max ) {

      hue = 0;
      saturation = 0;

    } else {

      var delta = max - min;

      saturation = lightness <= 0.5 ? delta / ( max + min ) : delta / ( 2 - max - min );

      switch ( max ) {

        case r: hue = ( g - b ) / delta + ( g < b ? 6 : 0 ); break;
        case g: hue = ( b - r ) / delta + 2; break;
        case b: hue = ( r - g ) / delta + 4; break;

      }

      hue /= 6;

    }

    hsl.h = hue;
    hsl.s = saturation;
    hsl.l = lightness;

    return hsl;
  }
#END OF SAMPLE  
}

// tests

function verifyOutput(rgb, hsl){
  var state = new Sample3(rgb.r, rgb.g, rgb.b)
  var out = {}
  var res1 = state.getHSL(out)
  var res2 = state.getHSL()
  assert(nearlyEqual(out.h, hsl.h))
  assert(nearlyEqual(res1.h, hsl.h))
  assert(nearlyEqual(res2.h, hsl.h))
  assert(nearlyEqual(out.s, hsl.s))
  assert(nearlyEqual(res1.s, hsl.s))
  assert(nearlyEqual(res2.s, hsl.s))
  assert(nearlyEqual(out.l, hsl.l))
  assert(nearlyEqual(res1.l, hsl.l))
  assert(nearlyEqual(res2.l, hsl.l))
}

// min != max
// (min+max)/2 > 0.5
// max == b
verifyOutput({ r: 0.1, g: 0.8, b: 1.0 }, { h: 0.5370370370370371, s: 1, l: 0.55 })

// min != max
// (min+max)/2 > 0.5
// max == g
verifyOutput({ r: 0.1, g: 1.0, b: 0.7 }, { h: 0.4444444444444444, s: 1, l: 0.55 })

// min != max
// (min+max)/2 > 0.5
// max == r
// g >= b
verifyOutput({ r: 0.9, g: 0.8, b: 0.7 }, { h: 0.08333333333333337, s: 0.5, l: 0.8 })

// min != max
// (min+max)/2 > 0.5
// max == r
// g < b
verifyOutput({ r: 0.9, g: 0.6, b: 0.7 }, { h: 0.9444444444444445, s: 0.6, l: 0.75 })

// min != max
// (min+max)/2 <= 0.5
// max == b
verifyOutput({ r: 0.1, g: 0.3, b: 0.6 }, { h: 0.6, s: 0.7142857142857143, l: 0.35 })

// min != max
// (min+max)/2 <= 0.5
// max == g
verifyOutput({ r: 0.15, g: 0.4, b: 0.2 }, { h: 0.3666666666666667, s: 0.45454545454545453, l: 0.275 })

// min != max
// (min+max)/2 <= 0.5
// max == r
// g >= b
verifyOutput({ r: 0.6, g: 0.3, b: 0.2 }, { h: 0.041666666666666664, s: 0.49999999999999994, l: 0.4 })

// min != max
// (min+max)/2 <= 0.5
// max == r
// g < b
verifyOutput({ r: 0.6, g: 0.1, b: 0.25 }, { h: 0.9500000000000001, s: 0.7142857142857143, l: 0.35 })

// min == max
verifyOutput({ r: 0.2, g: 0.2, b: 0.2 }, { h: 0, s: 0, l: 0.2 })
