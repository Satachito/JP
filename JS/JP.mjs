export const
TaggedElements = ( $, tag ) => Array.from( $.getElementsByTagName( tag ) )

export const
RangedFetch = ( url, range ) => fetch(
	url
,	{	headers:
		{	'content-type'	: 'multipart/byteranges'
		,	'range'			: 'bytes=' + range
		}
	}
).then(
	$ => $.arrayBuffer()
)

const
Char_4Bits = $ => {
	switch ( $ ) {
	case  0: return '0'
	case  1: return '1'
	case  2: return '2'
	case  3: return '3'
	case  4: return '4'
	case  5: return '5'
	case  6: return '6'
	case  7: return '7'
	case  8: return '8'
	case  9: return '9'
	case 10: return 'A'
	case 11: return 'B'
	case 12: return 'C'
	case 13: return 'D'
	case 14: return 'E'
	case 15: return 'F'
	default: throw 'eh?'
	}
}

//	Uint8 -> String
export const
CharHex = $ => Char_4Bits( $ >> 4 ) + Char_4Bits( $ & 0x0f )

//	Uint8Array -> String
export const
Hex = $ => $.reduce( ( $, _ ) => $ + CharHex( _ ), '' )
export const
Base64 = $ => btoa( String.fromCharCode( ...$ ) )
export const
Base64URL = $ => Base64( $ ).replace( /\+/g, '-' ).replace( /\//g, '_' ).replace( /=/g, '' )

//	String -> Uint8Array
export const
DecodeBase64 = $ => Uint8Array.from( atob( $ ), $ => $.charCodeAt( 0 ) )		
export const
DecodeBase64URL = $ => {
	let _ = $.replace( /-/g, '+' ).replace( /_/g, '/' )
	switch ( _ % 3 ) {
	case 0:	_ += '='
	case 2:	_ += '=='
	}
	return DecodeBase64( _ )
}

export const
DecodeHex = $ => {
	const v = new Uint8Array( $.length / 2 )
	v.forEach( ( _, i ) => v[ i ] = parseInt( $.substr( i * 2, 2 ), 16 ) )
	return v
}

export const
OnDef = ( $, def ) => $ === void 0 ? $ : def( $ )

export const
OnDefUndef = ( $, def, undef ) => $ === void 0 ? undef() : def( $ )

const
Seek = ( bias, $, length ) => {
	if ( bias ) {
		$ += bias
		if ( bias > 0 ) {
			while ( $ >= length ) $ -= length
		} else {
			while ( $ < 0 ) $ += length
		}
	}
	return $
}

const
FW = ( $, length ) => $ + 1 === length ? 0 : $ + 1

const
BW = ( $, length ) => ( $ ? $ : length ) - 1

const
Range = ( s, e, length ) => s < e
?   Array.from( { length: e - s }, ( _, k ) => k + s )
:   [   ...Array.from( { length: length - s }, ( _, k ) => k + s )
	,   ...Array.from( { length: e }, ( _, k ) => k )
	]

export default
() => {
	const
	a = DecodeHex( '7afe1822b54b4baf8b85d6e7f6c207ec' )

	const
	b = Hex( a )
	const
	c = Base64( a )
	const
	d = Base64URL( a )
	console.log( a, b, c, d )

	console.log(
		DecodeHex( b )
	,	DecodeBase64( c )
	,	DecodeBase64URL( d )
	)

	console.log( Seek( 5, 1, 3 ) )	//	0
	console.log( Seek( -5, 1, 3 ) )	//	2
	console.log( FW( 2, 3 ) )	//	0
	console.log( BW( 0, 3 ) )	//	2
	console.log( Range( 1, 3, 4 ) )	//	[ 1, 2 ]
	console.log( Range( 3, 1, 4 ) )	//	[ 3, 0 ]
}
