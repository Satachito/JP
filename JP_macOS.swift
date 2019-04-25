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
Input(
	_ pView: NSView
,	_ pMessage: String = ""
,	_ pInfomative: String = ""
) -> Bool {
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
OKCancel( _ pMessage: String, _ pInfomative: String,_ ed: () -> () ) {
	let	w = NSAlert()
	w.messageText = pMessage
	w.informativeText = pInfomative
	w.alertStyle = .warning
	w.addButton( withTitle: "OK" )
	w.addButton( withTitle: "Cancel" )
	if w.runModal() == .alertFirstButtonReturn { ed() }
}

func
YesNo( _ pMessage: String, _ pInfomative: String, _ yesed: () -> (), _ noed: () -> () ) {
	let	w = NSAlert()
	w.messageText = pMessage
	w.informativeText = pInfomative
	w.alertStyle = .warning
	w.addButton( withTitle: "OK" )
	w.addButton( withTitle: "Cancel" )
	switch w.runModal() {
	case .alertFirstButtonReturn	: yesed()
	case .alertSecondButtonReturn	: noed()
	default							: break
	}
}

class
V	: NSView {
	var drawer	: ( CGRect ) -> () = { _ in }
	override func
	draw( _ p: CGRect ) {
		super.draw( p )
		drawer( p )
	}
}

extension
NSColor {
	convenience init?( _ p: String, alpha: CGFloat ) {
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

class
SVGBezierPath: NSBezierPath {
	
	required init?(coder aDecoder: NSCoder) {
		fatalError("init(coder:) has not been implemented")
	}

	let	props	: [ String: String ]
	func
	ReadCGFloat( _ r: Reader< UnicodeScalar > ) throws -> CGFloat {
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
		while let w = r.Read() {
			if Valid( w ) {
				r.Unread( w )
				break
			}
		}
		while let w = r.Read() {
			if !Valid( w ) || ( w == "-" && v.count > 0 ) {
				r.Unread( w )
				break
			}
			v += String( w )
		}
		enum ERR: Error { case EOF }
		guard let w = Double( v ) else { throw ERR.EOF }
		return CGFloat( w )
	}
	func
	ReadPoint( _ r: Reader< UnicodeScalar > ) throws -> NSPoint {
		return NSPoint( x: try ReadCGFloat( r ), y: try ReadCGFloat( r ) )
	}
	func
	ReadSize( _ r: Reader< UnicodeScalar > ) throws -> NSSize {
		return NSSize( width: try ReadCGFloat( r ), height: try ReadCGFloat( r ) )
	}
	
	init( _ props: [ String: String ] ) {
		self.props = props
		super.init()
	}

	convenience init( points: String, _ props: [ String: String ] ) throws {
		self.init( props )
		let	r = StringUnicodeReader( points )

		enum ERR: Error { case Invalid }
		guard let wMT = try? ReadPoint( r ) else { throw ERR.Invalid }
		move( to: wMT )
		while let w = try? ReadPoint( r ) { line( to: w ) }
	}

	convenience init( d: String, _ props: [ String: String ] ) throws {
		self.init( props )

		let	r	= StringUnicodeReader( d )
		var	wC	: NSPoint?

		func
		Body( _ us: UnicodeScalar ) throws {
			switch us {
			case "Z", "z"	: wC = nil; close()
			case "M"		: wC = nil; move( to: try ReadPoint( r ) )
			case "m"		: wC = nil; move( to: try isEmpty ? ReadPoint( r ) : currentPoint + ReadSize( r ) )
			case "L"		: wC = nil; line( to: try ReadPoint( r ) )
			case "l"		: wC = nil; line( to: try currentPoint + ReadSize( r ) )
			case "H"		: wC = nil; line( to: NSPoint( x: try ReadCGFloat( r ), y: currentPoint.y ) )
			case "h"		: wC = nil; line( to: NSPoint( x: try currentPoint.x + ReadCGFloat( r ), y: currentPoint.y ) )
			case "V"		: wC = nil; line( to: NSPoint( x: currentPoint.x, y: try ReadCGFloat( r ) ) )
			case "v"		: wC = nil; line( to: NSPoint( x: currentPoint.x, y: try currentPoint.y + ReadCGFloat( r ) ) )
			case "C":
				let	wC1 = try ReadPoint( r )
				wC = try ReadPoint( r )
				curve( to: try ReadPoint( r ), controlPoint1: wC1, controlPoint2: wC! )
			case "Q":
				wC = try ReadPoint( r )
				quad( to: try ReadPoint( r ), controlPoint: wC! )
			case "S":
				var	wC1 = currentPoint
				if let w = wC { wC1 = currentPoint + ( currentPoint - w ) }
				wC = try ReadPoint( r )
				curve( to: try ReadPoint( r ), controlPoint1: wC1, controlPoint2: wC! )
			case "T":
				if let w = wC { wC = currentPoint + ( currentPoint - w ) } else { wC = currentPoint }
				quad( to: try ReadPoint( r ), controlPoint: wC! )
			case "c":
				let	wC1 = try currentPoint + ReadSize( r )
				wC = try currentPoint + ReadSize( r )
				curve( to: try currentPoint + ReadSize( r ), controlPoint1: wC1, controlPoint2: wC! )
			case "q":
				wC = try currentPoint + ReadSize( r )
				quad( to: try currentPoint + ReadSize( r ), controlPoint: wC! )
			case "s":
				var	wC1 = currentPoint
				if let w = wC { wC1 = currentPoint + ( currentPoint - w ) }
				wC = try currentPoint + ReadSize( r )
				curve( to: try currentPoint + ReadSize( r ), controlPoint1: wC1, controlPoint2: wC! )
			case "t":
				if let w = wC { wC = currentPoint + ( currentPoint - w ) } else { wC = currentPoint }
				quad( to: try currentPoint + ReadSize( r ), controlPoint: wC! )
			default:
				fatalError()
			}
		}

		var	wLastCommand	: UnicodeScalar?
		while true {
			SkipWhite( r )
			guard let w = r.Read() else { break }
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
		enum ERR: Error { case NoPath }
		guard elementCount > 0 else { throw ERR.NoPath }
		guard !isEmpty else { throw ERR.NoPath }
	}
	
	func
	Draw() {
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

	class func
	Parse( _ p: String ) throws -> [ SVGBezierPath ] {
		func
		Attributes( _ p: XMLElement ) -> [ String: String ] {
			return ( p.attributes ?? [] ).reduce( [ String: String ]() ) {
				$0.merging( [ $1.name!.lowercased(): $1.stringValue! ] ) { $1 }
			}
		}
		func
		Crawl(
			_ p: XMLElement
		,	_ a: [ String: String ]
		,	_ path: ( [ String: String ] ) -> ()
		,	_ polygon: ( [ String: String ] ) -> ()
		,	_ polyline: ( [ String: String ] ) -> ()
		) {
			let	wA = a.merging( Attributes( p ) ) { $1 }

			switch p.name {
			case "path"		: path( wA )
			case "polygon"	: polygon( wA )
			case "polyline"	: polyline( wA )
			default			: break
			}
			for c in p.children ?? [] {
				if let w = c as? XMLElement { Crawl( w, wA, path, polygon, polyline ) }
			}
		}
		enum ERR: Error {
			case	NoUTF8
			case	NoXML
			case	NoRoot
			case	NoSVG
		}
		guard let wData = DataByUTF8( p ) else { throw ERR.NoUTF8 }
		guard let wDOC = try? XMLDocument( data: wData ) else { throw ERR.NoXML }
		guard let wRoot = wDOC.rootElement() else { throw ERR.NoRoot }
		guard wRoot.name == "svg" else { throw ERR.NoSVG }
		var	v = [ SVGBezierPath ]()
		Crawl(
			wRoot
		,	[:]
		,	{	if let wS = $0[ "d" ], let w = try? SVGBezierPath( d: wS, $0.filter{ $0.key != "points" } ), w.elementCount > 0 {
					v.append( w )
				}
			}
		,	{	if let wS = $0[ "points" ], let w = try? SVGBezierPath( points: wS, $0.filter{ $0.key != "points" } ), w.elementCount > 0 {
					w.close()
					v.append( w )
				}
			}
		,	{	if let wS = $0[ "points" ], let w = try? SVGBezierPath( points: wS, $0.filter{ $0.key != "points" } ), w.elementCount > 0 {
					v.append( w )
				}
			}
		)
		return v
	}
}

import	WebKit

class
SVGGetter: NSObject, WKNavigationDelegate {
	var wv	= WKWebView()
	override init() {
		super.init()
		wv.navigationDelegate = self
	}
	var	cb	= { ( _: String ) in }
	func
	LoadFileURL( _ url: URL, _ cb: @escaping ( String ) -> () ) {
		self.cb = cb
		wv.loadFileURL( url, allowingReadAccessTo: url )
	}
	func
	LoadHTMLString( _ p: String, _ cb: @escaping ( String ) -> () ) {
		self.cb = cb
		wv.loadHTMLString( p, baseURL: nil )
	}
	func
	webView(_ webView: WKWebView, didFinish navigation: WKNavigation! ) {
		wv.evaluateJavaScript( "document.querySelector( 'svg' ).outerHTML" ) { a, e in
			if let w = e { Error( w ) }
			if let w = a as? String { self.cb( w ) }
		}
	}
}

