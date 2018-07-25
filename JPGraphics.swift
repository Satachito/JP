//	Written by Satoru Ogura, Tokyo.
//
import	Foundation
import	CoreGraphics
import	QuartzCore
import	Accelerate

func
ARGBContext( _ pW: size_t, _ pH: size_t ) -> CGContext? {
	return CGContext(
		data			: nil
	,	width			: pW
	,	height			: pH
	,	bitsPerComponent: 8
	,	bytesPerRow		: 0
	,	space			: CGColorSpaceCreateDeviceRGB()
	,	bitmapInfo		: CGImageAlphaInfo.premultipliedFirst.rawValue | CGBitmapInfo.byteOrder32Little.rawValue
	)
}

func
GrayContext( _ pW: size_t, _ pH: size_t ) -> CGContext? {
	return CGContext(
		data			: nil
	,	width			: pW
	,	height			: pH
	,	bitsPerComponent: 8
	,	bytesPerRow		: 0
	,	space			: CGColorSpaceCreateDeviceGray()
	,	bitmapInfo		: CGImageAlphaInfo.none.rawValue
	)
}

class
ARGBBitmap {
	let	m: CGContext
	init( _ pW: size_t, _ pH: size_t ) {
		m = ARGBContext( pW, pH )!
	}
	func
	Clear() {
		var	wZero = Int32( 0 )
		vDSP_vfilli(
			&wZero
		,	UnsafeMutablePointer<Int32>( OpaquePointer( m.data! ) )
		,	1
		,	vDSP_Length( m.bytesPerRow / 4 * m.height )
		)
	}
	func
	Pixel( _ pX: size_t, _ pY: size_t ) -> Int32 {
		guard 0 <= pX && pX < m.width && 0 <= pY && pY < m.height else { return 0 }
		return UnsafeMutablePointer<Int32>( OpaquePointer( m.data! ) )[ pY * m.bytesPerRow / 4 + pX ]
	}
	func
	Pixel( _ pX: size_t, _ pY: size_t, p: Int32 ) {
		guard 0 <= pX && pX < m.width && 0 <= pY && pY < m.height else { fatalError() }
		UnsafeMutablePointer<Int32>( OpaquePointer( m.data! ) )[ pY * m.bytesPerRow / 4 + pX ] = p
	}
	func
	HLine( _ pY: size_t, _ pSX: size_t, _ pEX: size_t, p: Int32 ) {
		guard 0 <= pY && pY < m.height else { return }
		var	wSX = pSX < 0 ? 0 : pSX < m.width ? pSX : m.width
		var	wEX = pEX < 0 ? 0 : pEX < m.width ? pEX : m.width
		if wSX > wEX { ( wSX, wEX ) = ( wEX, wSX ) }
		let wLength = wEX - wSX
		guard wLength > 0 else { return }

		var w = p
		vDSP_vfilli(
			&w
		,	UnsafeMutablePointer<Int32>( OpaquePointer( m.data! ) ) + m.bytesPerRow / 4 * pY + wSX
		,	1
		,	vDSP_Length( wLength )
		)
	}
	func
	VLine( _ pX: size_t, _ pSY: size_t, _ pEY: size_t, p: Int32 ) {
		guard 0 <= pX && pX < m.width else { return }
		var	wSY = pSY < 0 ? 0 : pSY < m.height ? pSY : m.height
		var	wEY = pEY < 0 ? 0 : pEY < m.height ? pEY : m.height
		if wSY > wEY { ( wSY, wEY ) = ( wEY, wSY ) }
		let wLength = wEY - wSY
		guard wLength > 0 else { return }

		var w = p
		vDSP_vfilli(
			&w
		,	UnsafeMutablePointer<Int32>( OpaquePointer( m.data! ) ) + m.bytesPerRow / 4 * wSY + pX
		,	m.bytesPerRow / 4
		,	vDSP_Length( wLength )
		)
	}
	func
	ShiftLeft() {
		let	wBytesPerRowDiv4 = m.bytesPerRow / 4
		let	wNumInts = wBytesPerRowDiv4 * m.height
		let w = UnsafeMutablePointer<Int32>( OpaquePointer( m.data! ) )
		memmove( w, w + 1, ( wNumInts - 1 ) * 4 );
		var	wZero = Int32( 0 )
		vDSP_vfilli( &wZero, w + wBytesPerRowDiv4 - 1, wBytesPerRowDiv4, vDSP_Length( m.height ) );
	}
	func
	ShiftRight() {
		let	wBytesPerRowDiv4 = m.bytesPerRow / 4
		let	wNumInts = wBytesPerRowDiv4 * m.height
		let w = UnsafeMutablePointer<Int32>( OpaquePointer( m.data! ) )
		memmove( w + 1, w, ( wNumInts - 1 ) * 4 );
		var	wZero = Int32( 0 )
		vDSP_vfilli( &wZero, w, wBytesPerRowDiv4, vDSP_Length( m.height ) );
	}
	func
	ShiftDown() {
		let	wBytesPerRowDiv4 = m.bytesPerRow / 4
		let	wNumInts = wBytesPerRowDiv4 * m.height
		let w = UnsafeMutablePointer<Int32>( OpaquePointer( m.data! ) )
		memmove( w, w + 1, ( wNumInts - wBytesPerRowDiv4 ) * 4 );
		var	wZero = Int32( 0 )
		vDSP_vfilli( &wZero, w + wNumInts - wBytesPerRowDiv4, 1, vDSP_Length( wBytesPerRowDiv4 ) );
	}
	func
	ShiftUp() {
		let	wBytesPerRowDiv4 = m.bytesPerRow / 4
		let	wNumInts = wBytesPerRowDiv4 * m.height
		let w = UnsafeMutablePointer<Int32>( OpaquePointer( m.data! ) )
		memmove( w + 1, w, ( wNumInts - wBytesPerRowDiv4 ) * 4 );
		var	wZero = Int32( 0 )
		vDSP_vfilli( &wZero, w, 1, vDSP_Length( wBytesPerRowDiv4 ) );
	}
}

/*
	sample:
	ColorWheelImage(
		size_t( bounds.width * UIScreen.main.scale )
	,	size_t( bounds.height * UIScreen.main.scale )
	)
*/
func
ColorWheelImage( _ pW: size_t, _ pH: size_t ) -> CGImage {

	func
	X( _ p: Double ) -> UInt32 {
		var	v = abs( p )
		if v < .pi * 1 / 3 { return 255 }
		v -= .pi * 1 / 3
		if v < .pi * 1 / 3 { return 255  - UInt32( v / ( .pi * 1 / 3 ) * 255 ) }
		return 0
//		var	v = abs( p ) / .pi * 3 / 2
//		if v > 1 { v = 1 }
//		return ( 255 - UInt32( v * 255 ) )
	}
	func
	R( _ p: Double ) -> UInt32 {
		return X( p ) << 16
	}
	func
	G( _ p: Double ) -> UInt32 {
		var	w = p - .pi * 2 / 3
		if w < -.pi { w += 2 * .pi }
		return X( w ) << 8
	}
	func
	B( _ p: Double ) -> UInt32 {
		var	w = p + .pi * 2 / 3
		if w > .pi { w -= 2 * .pi }
		return X( w )
	}

	guard let wARGBC = ARGBContext( pW, pH ) else { fatalError() }
	guard let w = wARGBC.data else { fatalError() }
	let	wPtr = UnsafeMutablePointer<UInt32>( OpaquePointer( w ) )
	let	wOX = Double( pW ) / 2
	let	wOY = Double( pH ) / 2

	for y in 0 ..< pH {
		let	wLinePtr = wPtr + y * wARGBC.bytesPerRow / 4
		let	wY = ( Double( y ) - wOY ) / wOY
		for x in 0 ..< pW {
			let	wX = ( Double( x ) - wOX ) / wOX
			let	w = atan2( wX, wY )
			wLinePtr[ x ] = 0xff000000 + R( w ) + G( w ) + B( w )
		}
	}

	guard let v = wARGBC.makeImage() else { fatalError() }
	return v
}

/*	Sample:

shapeMask.path = Dashed(
	outerPath.cgPath
,	size_t( bounds.width )
,	size_t( bounds.height )
,	10
,	( 0, [ 10, 10 ] )
)

*/

func
Dashed(
	_ p				: CGPath
,	_ pW			: size_t
,	_ pH			: size_t
,	_ pLineWidth	: CGFloat
,	_ pLineDash		: ( CGFloat, [ CGFloat ] )
,	_ pCap			: CGLineCap = .butt
,	_ pJoin			: CGLineJoin = .bevel
,	_ pMiterLimit	: CGFloat = 0
) -> CGPath {
	guard let v = ARGBContext( pW, pH ) else { fatalError() }
	v.addPath( p )
	v.setLineWidth( pLineWidth )
	v.setLineDash( phase: pLineDash.0, lengths: pLineDash.1 )
	v.setLineCap( pCap )
	v.setLineJoin( pJoin )
	v.setMiterLimit( pMiterLimit )
	v.replacePathWithStrokedPath()
	return v.path!
}
