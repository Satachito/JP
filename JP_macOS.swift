import Cocoa

func
Warning( _ pMessage: String, _ pInfomative: String = "" ) {
	let	w = NSAlert()
	w.messageText = pMessage
	w.informativeText = pInfomative
	w.alertStyle = .warning
	w.addButton( withTitle: "OK" )
	w.runModal()
}

func
Error( _ p: Error ) {
	let	w = NSAlert()
	w.messageText = "Error"
	w.informativeText = p.localizedDescription
	w.alertStyle = .critical
	w.addButton( withTitle: "OK" )
	w.runModal()
}

func
Input( _ pView: NSView, _ pMessage: String = "", _ pInfomative: String = "" ) -> Bool {
	let	w = NSAlert()
	w.messageText = pMessage
	w.informativeText = pInfomative
	w.alertStyle = .informational
	w.addButton( withTitle: "OK" )
	w.addButton( withTitle: "Cancel" )
    w.accessoryView = pView
	return w.runModal() == .alertFirstButtonReturn
}

func
OKCancel( _ pMessage: String, _ pInfomative: String = "" ) -> Bool {
	let	w = NSAlert()
	w.messageText = pMessage
	w.informativeText = pInfomative
	w.alertStyle = .informational
	w.addButton( withTitle: "OK" )
	w.addButton( withTitle: "Cancel" )
	return w.runModal() == .alertFirstButtonReturn
}

func
YesNo( _ pMessage: String, _ pInfomative: String = "" ) -> Bool {
	let	w = NSAlert()
	w.messageText = pMessage
	w.informativeText = pInfomative
	w.alertStyle = .informational
	w.addButton( withTitle: "Yes" )
	w.addButton( withTitle: "No" )
	return w.runModal() == .alertFirstButtonReturn
}

class
V	: NSView {
				var		drawer	: ( CGRect ) -> () = { _ in }
	override	func	draw	( _ p: CGRect ) { drawer( p ) }
	//	If your custom view is a direct NSView subclass, you do not need to call super
}

extension
NSColor {
	convenience
	init?( _ p: String, alpha: CGFloat ) {
		if p.hasPrefix( "#" ) {
			let w = p.dropFirst()
			switch w.count {
			case 3:
				guard let wR = Int( w.prefix( 1 ), radix: 16 ) else { return nil }
				guard let wG = Int( w.suffix( 2 ).prefix( 1 ), radix: 16 ) else { return nil }
				guard let wB = Int( w.suffix( 1 ), radix: 16 ) else { return nil }
				let	wCs: [ CGFloat ] = [
					CGFloat( Double( wR ) / 15.0 )
				,	CGFloat( Double( wG ) / 15.0 )
				,	CGFloat( Double( wB ) / 15.0 )
				,	alpha
				]
				self.init( colorSpace: .deviceRGB, components: wCs, count: 4 )
			case 6:
				guard let wR = Int( w.prefix( 2 ), radix: 16 ) else { return nil }
				guard let wG = Int( w.suffix( 4 ).prefix( 2 ), radix: 16 ) else { return nil }
				guard let wB = Int( w.suffix( 2 ), radix: 16 ) else { return nil }
				let	wCs: [ CGFloat ] = [
					CGFloat( Double( wR ) / 255.0 )
				,	CGFloat( Double( wG ) / 255.0 )
				,	CGFloat( Double( wB ) / 255.0 )
				,	alpha
				]
				self.init( colorSpace: .deviceRGB, components: wCs, count: 4 )
			default:
				return nil
			}
		} else {
			guard let w = RGBIntColorValues[ p ] else { return nil }
			let	wCs: [ CGFloat ] = [
				CGFloat( w.0 ) / 255.0
			,	CGFloat( w.1 ) / 255.0
			,	CGFloat( w.2 ) / 255.0
			,	alpha
			]
			self.init( colorSpace: .deviceRGB, components: wCs, count: 4 )
		}
	}
	func
	Values() -> ( String, CGFloat ) {
		var	wR = CGFloat( 0 )
		var	wG = CGFloat( 0 )
		var	wB = CGFloat( 0 )
		var	wA = CGFloat( 0 )
		usingColorSpace( .deviceRGB )!.getRed( &wR, green: &wG, blue: &wB, alpha: &wA )
		let H = { ( p: CGFloat ) in String( format: "%02x", Int( p * 255.9999 ) ) }
		return ( "#\(H(wR))\(H(wG))\(H(wB))", wA )
	}
}

func
ColorName( _ p: String ) -> String? {
	let	wR = Int( p.prefix( 2 ), radix: 16 )
	let	wG = Int( p.suffix( 4 ).prefix( 2 ), radix: 16 )
	let	wB = Int( p.suffix( 2 ), radix: 16 )
	for ( key, value ) in RGBIntColorValues {
		if value.0 == wR && value.1 == wG && value.2 == wB { return key }
	}
	return nil
}

let
RGBIntColorValues = [
	"liceblue"				: ( 240, 248, 255 )
,	"antiquewhite"			: ( 250, 235, 215 )
,	"aqua"					: ( 0, 255, 255 )
,	"aquamarine"			: ( 127, 255, 212 )
,	"azure"					: ( 240, 255, 255 )
,	"beige"					: ( 245, 245, 220 )
,	"bisque"				: ( 255, 228, 196 )
,	"black"					: ( 0, 0, 0 )
,	"blanchedalmond"		: ( 255, 235, 205 )
,	"blue"					: ( 0, 0, 255 )
,	"blueviolet"			: ( 138, 43, 226 )
,	"brown"					: ( 165, 42, 42 )
,	"burlywood"				: ( 222, 184, 135 )
,	"cadetblue"				: ( 95, 158, 160 )
,	"chartreuse"			: ( 127, 255, 0 )
,	"chocolate"				: ( 210, 105, 30 )
,	"coral"					: ( 255, 127, 80 )
,	"cornflowerblue"		: ( 100, 149, 237 )
,	"cornsilk"				: ( 255, 248, 220 )
,	"crimson"				: ( 220, 20, 60 )
,	"cyan"					: ( 0, 255, 255 )
,	"darkblue"				: ( 0, 0, 139 )
,	"darkcyan"				: ( 0, 139, 139 )
,	"darkgoldenrod"			: ( 184, 134, 11 )
,	"darkgray"				: ( 169, 169, 169 )
,	"darkgreen"				: ( 0, 100, 0 )
,	"darkgrey"				: ( 169, 169, 169 )
,	"darkkhaki"				: ( 189, 183, 107 )
,	"darkmagenta"			: ( 139, 0, 139 )
,	"darkolivegreen"		: ( 85, 107, 47 )
,	"darkorange"			: ( 255, 140, 0 )
,	"darkorchid"			: ( 153, 50, 204 )
,	"darkred"				: ( 139, 0, 0 )
,	"darksalmon"			: ( 233, 150, 122 )
,	"darkseagreen"			: ( 143, 188, 143 )
,	"darkslateblue"			: ( 72, 61, 139 )
,	"darkslategray"			: ( 47, 79, 79 )
,	"darkslategrey"			: ( 47, 79, 79 )
,	"darkturquoise"			: ( 0, 206, 209 )
,	"darkviolet"			: ( 148, 0, 211 )
,	"deeppink"				: ( 255, 20, 147 )
,	"deepskyblue"			: ( 0, 191, 255 )
,	"dimgray"				: ( 105, 105, 105 )
,	"dimgrey"				: ( 105, 105, 105 )
,	"dodgerblue"			: ( 30, 144, 255 )
,	"firebrick"				: ( 178, 34, 34 )
,	"floralwhite"			: ( 255, 250, 240 )
,	"forestgreen"			: ( 34, 139, 34 )
,	"fuchsia"				: ( 255, 0, 255 )
,	"gainsboro"				: ( 220, 220, 220 )
,	"ghostwhite"			: ( 248, 248, 255 )
,	"gold"					: ( 255, 215, 0 )
,	"goldenrod"				: ( 218, 165, 32 )
,	"gray"					: ( 128, 128, 128 )
,	"green"					: ( 0, 128, 0 )
,	"greenyellow"			: ( 173, 255, 47 )
,	"grey"					: ( 128, 128, 128 )
,	"honeydew"				: ( 240, 255, 240 )
,	"hotpink"				: ( 255, 105, 180 )
,	"indianred"				: ( 205, 92, 92 )
,	"indigo"				: ( 75, 0, 130 )
,	"ivory"					: ( 255, 255, 240 )
,	"khaki"					: ( 240, 230, 140 )
,	"lavender"				: ( 230, 230, 250 )
,	"lavenderblush"			: ( 255, 240, 245 )
,	"lawngreen"				: ( 124, 252, 0 )
,	"lemonchiffon"			: ( 255, 250, 205 )
,	"lightblue"				: ( 173, 216, 230 )
,	"lightcoral"			: ( 240, 128, 128 )
,	"lightcyan"				: ( 224, 255, 255 )
,	"lightgoldenrodyellow"	: ( 250, 250, 210 )
,	"lightgray"				: ( 211, 211, 211 )
,	"lightgreen"			: ( 144, 238, 144 )
,	"lightgrey"				: ( 211, 211, 211 )
,	"lightpink"				: ( 255, 182, 193 )
,	"lightsalmon"			: ( 255, 160, 122 )
,	"lightseagreen"			: ( 32, 178, 170 )
,	"lightskyblue"			: ( 135, 206, 250 )
,	"lightslategray"		: ( 119, 136, 153 )
,	"lightslategrey"		: ( 119, 136, 153 )
,	"lightsteelblue"		: ( 176, 196, 222 )
,	"lightyellow"			: ( 255, 255, 224 )
,	"lime"					: ( 0, 255, 0 )
,	"limegreen"				: ( 50, 205, 50 )
,	"linen"					: ( 250, 240, 230 )
,	"magenta"				: ( 255, 0, 255 )
,	"maroon"				: ( 128, 0, 0 )
,	"mediumaquamarine"		: ( 102, 205, 170 )
,	"mediumblue"			: ( 0, 0, 205 )
,	"mediumorchid"			: ( 186, 85, 211 )
,	"mediumpurple"			: ( 147, 112, 219 )
,	"mediumseagreen"		: ( 60, 179, 113 )
,	"mediumslateblue"		: ( 123, 104, 238 )
,	"mediumspringgreen"		: ( 0, 250, 154 )
,	"mediumturquoise"		: ( 72, 209, 204 )
,	"mediumvioletred"		: ( 199, 21, 133 )
,	"midnightblue"			: ( 25, 25, 112 )
,	"mintcream"				: ( 245, 255, 250 )
,	"mistyrose"				: ( 255, 228, 225 )
,	"moccasin"				: ( 255, 228, 181 )
,	"navajowhite"			: ( 255, 222, 173 )
,	"navy"					: ( 0, 0, 128 )
,	"oldlace"				: ( 253, 245, 230 )
,	"olive"					: ( 128, 128, 0 )
,	"olivedrab"				: ( 107, 142, 35 )
,	"orange"				: ( 255, 165, 0 )
,	"orangered"				: ( 255, 69, 0 )
,	"orchid"				: ( 218, 112, 214 )
,	"palegoldenrod"			: ( 238, 232, 170 )
,	"palegreen"				: ( 152, 251, 152 )
,	"paleturquoise"			: ( 175, 238, 238 )
,	"palevioletred"			: ( 219, 112, 147 )
,	"papayawhip"			: ( 255, 239, 213 )
,	"peachpuff"				: ( 255, 218, 185 )
,	"peru"					: ( 205, 133, 63 )
,	"pink"					: ( 255, 192, 203 )
,	"plum"					: ( 221, 160, 221 )
,	"powderblue"			: ( 176, 224, 230 )
,	"purple"				: ( 128, 0, 128 )
,	"rebeccapurple"			: ( 102, 51, 153 )
,	"red"					: ( 255, 0, 0 )
,	"rosybrown"				: ( 188, 143, 143 )
,	"royalblue"				: ( 65, 105, 225 )
,	"saddlebrown"			: ( 139, 69, 19 )
,	"salmon"				: ( 250, 128, 114 )
,	"sandybrown"			: ( 244, 164, 96 )
,	"seagreen"				: ( 46, 139, 87 )
,	"seashell"				: ( 255, 245, 238 )
,	"sienna"				: ( 160, 82, 45 )
,	"silver"				: ( 192, 192, 192 )
,	"skyblue"				: ( 135, 206, 235 )
,	"slateblue"				: ( 106, 90, 205 )
,	"slategray"				: ( 112, 128, 144 )
,	"slategrey"				: ( 112, 128, 144 )
,	"snow"					: ( 255, 250, 250 )
,	"springgreen"			: ( 0, 255, 127 )
,	"steelblue"				: ( 70, 130, 180 )
,	"tan"					: ( 210, 180, 140 )
,	"teal"					: ( 0, 128, 128 )
,	"thistle"				: ( 216, 191, 216 )
,	"tomato"				: ( 255, 99, 71 )
,	"turquoise"				: ( 64, 224, 208 )
,	"violet"				: ( 238, 130, 238 )
,	"wheat"					: ( 245, 222, 179 )
,	"white"					: ( 255, 255, 255 )
,	"whitesmoke"			: ( 245, 245, 245 )
,	"yellow"				: ( 255, 255, 0 )
,	"yellowgreen"			: ( 154, 205, 50 )
]


func
-( l: NSPoint, r: NSPoint ) -> NSSize {
	return NSSize( width: l.x - r.x, height: l.y - r.y )
}

func
+( p: NSPoint, s: NSSize ) -> NSPoint {
	return NSPoint( x: p.x + s.width, y: p.y + s.height )
}

func
-( p: NSPoint, s: NSSize ) -> NSPoint {
	return NSPoint( x: p.x - s.width, y: p.y - s.height )
}

func
!=( l: NSPoint, r: NSPoint ) -> Bool {
//print( abs( l.x - r.x ), abs( l.y - r.y ) )
	return NSEqualPoints( l, r )
}

func
Mid( _ l: NSPoint, _ r: NSPoint ) -> NSPoint {
	return NSPoint( x: ( l.x + r.x ) / 2, y: ( l.y + r.y ) / 2 )
}

//

extension
NSRect {
	init( _ p: NSPoint ) {
		self.init( origin: p, size: NSSize.zero )
	}
}

func
|( _ l: NSRect, _ r: NSRect ) -> NSRect {
	return l.union( r )
}

func
|( _ r: NSRect, _ c: NSPoint ) -> NSRect {
	return r | NSRect( c )
}

func
|( _ c: NSPoint, _ r: NSRect ) -> NSRect {
	return NSRect( c ) | r
}

//

extension
NSBezierPath {
	func
	quad( to endPoint: NSPoint, controlPoint: NSPoint ) {
		func
		ToT( _ c: NSPoint, _ a: NSPoint ) -> NSPoint {
			return NSPoint( x:( c.x * 2 + a.x ) / 3, y: ( c.y * 2 + a.y ) / 3 )
		}
		curve( to: endPoint, controlPoint1: ToT( controlPoint, currentPoint ), controlPoint2: ToT( controlPoint, endPoint ) )
	}
}

extension
StringUnicodeReader {
	func
	CGFloatValue() throws -> CGFloat {
		func
		Valid( _ us: UnicodeScalar ) -> Bool {
			switch us {
			case "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ".", "-":
				return true
			default:
				return false
			}
		}
		var	v = ""
		while let w = try? Read() {
			if Valid( w ) {
				Unread( w )
				break
			}
		}
		while let w = try? Read() {
			if !Valid( w ) || ( w == "-" && v.count > 0 ) {
				Unread( w )
				break
			}
			v += String( w )
		}
		enum ERR: Error { case Invalid }
		guard let w = Double( v ) else { throw ERR.Invalid }
		return CGFloat( w )
	}
	func
	NSPointValue() throws -> NSPoint {
		return NSPoint( x: try CGFloatValue(), y: try CGFloatValue() )
	}
	func
	NSSizeValue() throws -> NSSize {
		return NSSize( width: try CGFloatValue(), height: try CGFloatValue() )
	}
}


extension
NSBezierPath {
	
	convenience
	init( points: String ) throws {
		self.init()
		let	r = StringUnicodeReader( points )
		move( to: try r.NSPointValue() )
		while let w = try? r.NSPointValue() { line( to: w ) }
	}

	convenience
	init( d: String ) throws {
		self.init()

		let	r	= StringUnicodeReader( d )
		var	wC	: NSPoint?

		func
		Body( _ us: UnicodeScalar ) throws {
			switch us {
			case "Z", "z"	: wC = nil; close()
			case "M"		: wC = nil; move( to: try r.NSPointValue() )
			case "m"		: wC = nil; move( to: try isEmpty ? r.NSPointValue() : currentPoint + r.NSSizeValue() )
			case "L"		: wC = nil; line( to: try r.NSPointValue() )
			case "l"		: wC = nil; line( to: try currentPoint + r.NSSizeValue() )
			case "H"		: wC = nil; line( to: NSPoint( x: try r.CGFloatValue(), y: currentPoint.y ) )
			case "h"		: wC = nil; line( to: NSPoint( x: try currentPoint.x + r.CGFloatValue(), y: currentPoint.y ) )
			case "V"		: wC = nil; line( to: NSPoint( x: currentPoint.x, y: try r.CGFloatValue() ) )
			case "v"		: wC = nil; line( to: NSPoint( x: currentPoint.x, y: try currentPoint.y + r.CGFloatValue() ) )
			case "C":
				let	wC1 = try r.NSPointValue()
				wC = try r.NSPointValue()
				curve( to: try r.NSPointValue(), controlPoint1: wC1, controlPoint2: wC! )
			case "Q":
				wC = try r.NSPointValue()
				quad( to: try r.NSPointValue(), controlPoint: wC! )
			case "S":
				var	wC1 = currentPoint
				if let w = wC { wC1 = currentPoint + ( currentPoint - w ) }
				wC = try r.NSPointValue()
				curve( to: try r.NSPointValue(), controlPoint1: wC1, controlPoint2: wC! )
			case "T":
				if let w = wC { wC = currentPoint + ( currentPoint - w ) } else { wC = currentPoint }
				quad( to: try r.NSPointValue(), controlPoint: wC! )
			case "c":
				let	wC1 = try currentPoint + r.NSSizeValue()
				wC = try currentPoint + r.NSSizeValue()
				curve( to: try currentPoint + r.NSSizeValue(), controlPoint1: wC1, controlPoint2: wC! )
			case "q":
				wC = try currentPoint + r.NSSizeValue()
				quad( to: try currentPoint + r.NSSizeValue(), controlPoint: wC! )
			case "s":
				var	wC1 = currentPoint
				if let w = wC { wC1 = currentPoint + ( currentPoint - w ) }
				wC = try currentPoint + r.NSSizeValue()
				curve( to: try currentPoint + r.NSSizeValue(), controlPoint1: wC1, controlPoint2: wC! )
			case "t":
				if let w = wC { wC = currentPoint + ( currentPoint - w ) } else { wC = currentPoint }
				quad( to: try currentPoint + r.NSSizeValue(), controlPoint: wC! )
			default:
				enum ERR: Error { case Invalid }
				throw ERR.Invalid
			}
		}

		var	wLastCommand	: UnicodeScalar?
		while let w = try? r.ReadNonWhite() {
			switch w {
			case "Z", "z", "M", "m", "L", "l", "H", "h", "V", "v", "C", "c", "Q", "q", "S", "s", "T", "t":
				try Body( w )
				wLastCommand = w
			default:
				r.Unread( w )
				if let wLC = wLastCommand {
					switch wLC {
					case "m": try Body( "l" )
					case "M": try Body( "L" )
					default	: try Body( wLC )
					}
				}
			}
		}
	}

	func
	Draw( _ props: [ String: String ] ) {
		var	wDashArray		: [ CGFloat ]?
		var	wDashOffset		= CGFloat( 0 )
		var	wFillColor		= "black"
		var	wFillOpacity	= CGFloat( 1 )
		var	wStrokeColor	= "none"
		var	wStrokeOpacity	= CGFloat( 1 )
		for ( key, value ) in props {
			switch key {
			case "fill":
				wFillColor = value
			case "fill-opacity":
				wFillOpacity = CGFloat( Double( value ) ?? 1 )
			case "fill-rule":
				switch value {
				case "nonzero"	: windingRule = .nonZero
				case "evenodd"	: windingRule = .evenOdd
				default			: break
				}
			case "stroke":
				wStrokeColor = value
			case "stroke-opacity":
				wStrokeOpacity = CGFloat( Double( value ) ?? 1 )
			case "stroke-width":
				lineWidth = CGFloat( Double( value ) ?? 1 )
			case "stroke-linecap":
				switch value {
				case "butt"		: lineCapStyle = .butt
				case "round"	: lineCapStyle = .round
				case "square"	: lineCapStyle = .square
				default			: break
				}
			case "stroke-linejoin":
				switch value {
				case "miter"	: lineJoinStyle = .miter
				case "round"	: lineJoinStyle = .round
				case "bevel"	: lineJoinStyle = .bevel
				default			: break
				}
			case "stroke-dasharray":
				wDashArray = value.components( separatedBy: .whitespaces ).map { CGFloat( Double( $0 ) ?? 0 ) }
			case "stroke-dashoffset":
				wDashOffset = CGFloat( Double( value ) ?? 0 )
			default:
				break
			}
		}
		if let w = wDashArray { setLineDash( w, count: w.count, phase: wDashOffset ) }
		if let w = NSColor( wFillColor, alpha: wFillOpacity )		{ w.set(); fill() }
		if let w = NSColor( wStrokeColor, alpha: wStrokeOpacity )	{ w.set(); stroke() }
	}
}

import	WebKit

func
AsXML( _ p: Data ) throws -> XMLElement {
	enum ERR: Error {
		case	NoXML
		case	NoRoot
		case	NoSVG
	}
	guard let wDOC = try? XMLDocument( data: p ) else { throw ERR.NoXML }
	guard let v = wDOC.rootElement() else { throw ERR.NoRoot }
	guard v.name == "svg" else { throw ERR.NoSVG }
	return v
}
 
class
SVGGetter: NSObject, WKNavigationDelegate {
	var wv	= WKWebView()
	override init() {
		super.init()
		wv.navigationDelegate = self
	}
	var	cb	= { ( _: XMLElement?, _ : Error? ) in }
	func
	LoadFileURL( _ url: URL, _ cb: @escaping ( XMLElement?, Error? ) -> () ) {
		self.cb = cb
		wv.loadFileURL( url, allowingReadAccessTo: url )
	}
	func
	LoadHTMLString( _ p: String, _ cb: @escaping ( XMLElement?, Error? ) -> () ) {
		self.cb = cb
		wv.loadHTMLString( p, baseURL: nil )
	}
	func
	webView(_ webView: WKWebView, didFinish navigation: WKNavigation! ) {
		wv.evaluateJavaScript( "document.querySelector( 'svg' ).outerHTML" ) { a, e in
			if let w = e { Error( w ) }
			if let w = a as? String {
				guard let wData = DataByUTF8( w ) else { fatalError() }
				do { self.cb( AsXML( wData ), nil ) } catch ( e: Error ) { self.cb( nil, e ) }
			}
		}
	}
}

func
CrawlSVG(
	_ p: XMLElement
,	_ a: [ String: String ]
,	_ path: ( [ String: String ] ) -> ()
,	_ polygon: ( [ String: String ] ) -> ()
,	_ polyline: ( [ String: String ] ) -> ()
) {
	func
	AttrDict( _ p: XMLElement ) -> [ String: String ] {
		return ( p.attributes ?? [] ).reduce( [ String: String ]() ) {
			$0.merging( [ $1.name!.lowercased(): $1.stringValue! ] ) { $1 }
		}
	}
	let	wA = a.merging( AttrDict( p ) ) { $1 }

	switch p.name {
	case "path"		: path( wA )
	case "polygon"	: polygon( wA )
	case "polyline"	: polyline( wA )
	default			: break
	}
	for c in p.children ?? [] {
		if let w = c as? XMLElement { CrawlSVG( w, wA, path, polygon, polyline ) }
	}
}

func
DrawSVG( _ p: XMLElement, _ r: CGRect, _ a: AffineTransform ) {
	CrawlSVG(
		p
	,	[:]
	,	{	if let wD = $0[ "d" ], let w = try? NSBezierPath( d: wD ) {
				w.transform( using: a )
				if r.intersects( w.bounds ) { w.Draw( $0 ) } //else { print( NSDate(), "SKIP" ) }
			}
		}
	,	{	if let wPoints = $0[ "points" ], let w = try? NSBezierPath( points: wPoints ) {
				w.close()
				w.transform( using: a )
				if r.intersects( w.bounds ) { w.Draw( $0 ) } //else { print( NSDate(), "SKIP" ) }
			}
		}
	,	{	if let wPoints = $0[ "points" ], let w = try? NSBezierPath( points: wPoints ) {
				w.transform( using: a )
				if r.intersects( w.bounds ) { w.Draw( $0 ) } //else { print( NSDate(), "SKIP" ) }
			}
		}
	)
}

func
BBoxSVG( _ p: XMLElement ) -> NSRect {
	var	v = NSRect.null
	CrawlSVG(
		p
	,	[:]
	,	{ if let wS = $0[ "d" ]		, let w = try? NSBezierPath( d		: wS ) { v = v | w.bounds } }
	,	{ if let wS = $0[ "points" ], let w = try? NSBezierPath( points	: wS ) { v = v | w.bounds } }
	,	{ if let wS = $0[ "points" ], let w = try? NSBezierPath( points	: wS ) { v = v | w.bounds } }
	)
	return v
}
