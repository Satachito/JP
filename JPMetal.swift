//	Written by Satoru Ogura, Tokyo.
//
import Cocoa

class
TGAFile {

	let	rawData	: Data

	func
	UI8( _ i: Int ) -> UInt8 {
		var	v: UInt8 = 0
		rawData.withUnsafeBytes { p -> Void in v = p[ i ] }
		return v
	}
	func
	I16( _ i: Int ) -> Int16 {
		var	v: Int16 = 0
		rawData.withUnsafeBytes { p -> Void in
			v = UnsafeMutablePointer< Int16 >( OpaquePointer( p + i ) )[ 0 ]
		}
		return v
	}
	func
	UI16( _ i: Int ) -> UInt16 {
		var	v: UInt16 = 0
		rawData.withUnsafeBytes { p -> Void in
			v = UnsafeMutablePointer< UInt16 >( OpaquePointer( p + i ) )[ 0 ]
		}
		return v
	}
	
	func	IDSize() 		-> UInt8	{ return UI8 (  0 ) }	// Size of ID info following header
	func	colorMapType()	-> UInt8	{ return UI8 (  1 ) }	// Whether this is a paletted image
	func	imageType()		-> UInt8	{ return UI8 (  2 ) }	// type of image 0=none, 1=indexed, 2=rgb, 3=grey, +8=rle packed

	func	colorMapStart()	-> Int16	{ return  I16(  3 ) }	// Offset to color map in palette
	func	colorMapLength()-> Int16	{ return  I16(  5 ) }	// Number of colors in palette
	func	colorMapBpp()	-> UInt8	{ return UI8 (  7 ) }	// number of bits per palette entry

	func	xOffset()		-> UInt16	{ return UI16(  8 ) }	// Number of pixels to the right to start of image
	func	yOffset()		-> UInt16	{ return UI16( 10 ) }	// Number of pixels down to start of image
	func	width()			-> UInt16	{ return UI16( 12 ) }	// Width in pixels
	func	height()		-> UInt16	{ return UI16( 14 ) }	// Height in pixels
	func	bitsPerPixel()	-> UInt8	{ return UI8 ( 16 ) }	// Bits per pixel 8,16,24,32
	func	descriptor()	-> UInt8	{ return UI8 ( 17 ) }	// Descriptor bits (flipping, etc)

	func
	Image() -> [ UInt32 ] {
		let	wNumPixels = Int( width() ) * Int( height() )
		var	v = [ UInt32 ]( repeating: 0, count: wNumPixels )
		rawData.withUnsafeBytes { ( p: UnsafePointer< UInt8 > ) -> Void in
			switch bitsPerPixel() {
			case 24:
				for i in 0 ..< wNumPixels {
					let	w = i * 3
					v[ i ] = UInt32( p[ w ] ) << 24 + UInt32( p[ w + 1 ] ) << 16 + UInt32( p[ w + 2 ] ) << 8 + UInt32( 0x0ff )
				}
			case 32:
				for i in 0 ..< wNumPixels {
					v[ i ] = UnsafePointer< UInt32 >( OpaquePointer( p ) )[ i ]
				}
			default:
				break
			}
		}
		return v
	}

	init( url: URL ) {
		rawData = try! Data( contentsOf: url )
	}
}
