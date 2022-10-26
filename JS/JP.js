//	Written by Satoru Ogura, SliP LLC Tokyo.
//

export const
IsJSONable = _ => {
	if ( _ === void 0	) return false
	if ( _ === null		) return true
	switch ( _.constructor ) {
	case Array:
		return _.every( _ => IsJSONable( _ ) )
	case Object:
		return Object.entries( _ ).every( ( [ k, v ] ) => k.constructor === String && IsJSONable( v ) )
	case String:
		return true
	case Boolean:
		return true
	case Number:
		if ( isNaN( _ )						) return false
		if ( _ === Number.POSITIVE_INFINITY	) return false
		if ( _ === Number.NEGATIVE_INFINITY	) return false
		return true
	default:
		return false
	}
}

export const
CloneJSONable = _ => {

//	In JavaScript, strings are immutable.
//	Comparing to JSON.parse( JSON.stringify( _ ) ), this function is a little bit faster and efficient by string sharing.

	if ( _ === null ) return _
	
	switch ( _.constructor ) {
	case Array:
		return _.map( _ => CloneJSONable( _ ) )
	case Object:
		return Object.fromEntries(
			Object.entries( _ ).map( ( [ $, _ ] ) => [ $, CloneJSONable( _ ) ] )
		)
		break
	default:	// String, Boolean, Number
		return _
	}
}

export const
EqualJSONable = ( p, q ) => {
	if ( p === null ) return p === q
	if ( q === null ) return false
	
	const c = p.constructor
	if ( c !== q.constructor ) return false
	switch ( c ) {
	case Array:
		return p.length === q.length
		?	p.every( ( $, _ ) => EqualJSONable( $, q[ _ ] ) )
		:	false
	case Object:
		const pKeys = Object.keys( p )
		if ( pKeys.length !== Object.keys( q ).length ) return false
		return pKeys.every(
			_ => $ === q[ _ ] && EqualJSONable( p[ _ ], q[ _ ] )
		)
	default:
		return p === q
	}
}

export const
Last = _ => _[ _.length - 1 ]

export const
AND = ( l, r ) => l.filter( _ => !r.includes( _ ) )

export const
OR = ( l, r ) => [ ...l.filter( _ => !r.includes( _ ) ), r ]

export const
XOR = ( l, r ) => [ ...l.filter( _ => !r.includes( _ ) ), ...r.filter( _ => !l.includes( _ ) ) ]

//v	INPLACE
export const
Remove = ( $, _ ) => $.splice( $.findIndex( $ => $ === _ ), 1 )
//^

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
	if ( $.length % 2 ) $ = '0' + $
	const v = new Uint8Array( $.length / 2 )
	v.forEach( ( _, i ) => v[ i ] = parseInt( $.substr( i * 2, 2 ), 16 ) )
	return v
}

export const
OnDef = ( $, def ) => $ === void 0 ? $ : def( $ )

export const
OnDefUndef = ( $, def, undef ) => $ === void 0 ? undef() : def( $ )

export const
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

export const
FW = ( $, length ) => $ + 1 === length ? 0 : $ + 1

export const
BW = ( $, length ) => ( $ ? $ : length ) - 1

export const
Range = ( s, e, length ) => s < e
?   Array.from( { length: e - s }, ( _, k ) => k + s )
:   [   ...Array.from( { length: length - s }, ( _, k ) => k + s )
	,   ...Array.from( { length: e }, ( _, k ) => k )
	]


export default
() => {

	const _ = { A: [ void 0 ] }						; console.log( _, IsJSONable( _ ) )
	const X = { A: [ NaN ] }						; console.log( X, IsJSONable( X ) )
	const Y = { A: [ Number.POSITIVE_INFINITY ] }	; console.log( Y, IsJSONable( Y ) )
	const Z = { A: [ Number.NEGATIVE_INFINITY ] }	; console.log( Z, IsJSONable( Z ) )

	const
	P = {
		A: [ 1, "A", true ]
	,	B: [ 2, "B", false ]
	}
	console.log( P, IsJSONable( P ) )

	const
	Q = {
		B: [ 2, "B", false ]
	,	A: [ 1, "A", true ]
	}
	console.log( Q, IsJSONable( Q ) )

	console.log( P, Q, EqualJSONable( P, Q ) )

	console.log( EqualJSONable( CloneJSONable( P ), CloneJSONable( Q ) ) )
	console.log( EqualJSONable( JSON.parse( JSON.stringify( P ) ), P ) )


/*
	const
	TestValue = ( js, expected ) => {
		const _ = eval( js )
		EqualJSONable( _, expected )
		?	console.log( 'OK', js, _ )
		:	console.error( 'ERROR', js, _, expected )
	}

	TestValue( "EqualJSONable( [], [] )", true )
	TestValue( "EqualJSONable( null, null )", true )
	TestValue( "EqualJSONable( [ 1, 'A' ], [ 1, 'A' ] )", true )
	TestValue( "EqualJSONable( [ 1, 'A' ], [ 1, 'B' ] )", false )
	TestValue( "EqualJSONable( [ 1, 'A', 2 ], [ 1, 'A' ] )", false )
	TestValue( "EqualJSONable( { 'A': 1, 'B': 2 }, { 'B': 2, 'A': 1 } )", true )
	TestValue( "EqualJSONable( { 'A': 1, 'B': [ 1, 2 ] }, { 'B': [ 1, 2 ], 'A': 1 } )", true )
	
	const
	a = DecodeHex( 'afe1822b54b4baf8b85d6e7f6c207ec' )

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
*/
}
