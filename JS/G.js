export const
CF	= ( -24 + Math.sqrt( 24 * 24 + 64 * 9 ) ) / 18	//	Curve factor	: 0.552285

export const
EQ = ( p, q ) => p.every( ( $, _ ) => $ === q[ _ ] )

export const
Round = $ => $.map( $ => Math.round( $ ) )

export const
Near = ( p, q ) => p.every( ( $, _ ) => Math.abs( $ - q[ _ ] ) <= 1 )

export const
Add = ( p, q ) => p.map( ( $, _ ) => $ + q[ _ ] )

export const
Sub = ( p, q ) => p.map( ( $, _ ) => $ - q[ _ ] )

export const
Vec = ( p, q ) => p.map( ( $, _ ) => q[ _ ] - $ )

export const
Mul = ( $, _ ) => $.map( $ => $ * _ )

export const
Div = ( $, _ ) => $.map( $ => $ / _ )

export const
Mid = ( p, q ) => p.map( ( $, _ ) => ( $ + q[ _ ] ) / 2 )

export const
Dot = ( p, q ) => p.reduce( ( $, p, _ ) => $ + p * q[ _ ], 0 )

export const
Norm = $ => Math.sqrt( Dot( $, $ ) )

export const	//	3D
Cross3D = ( [ px, py, pz ], [ qx, qy, qz ] ) => [
	py * qz - pz * qy
,	pz * qx - px * qz
,	px * qy - py * qx
]

export const	//	3D
FormedAngle = ( p, q ) => Math.atan2( Norm( Cross3D( p, q ) ), Dot( p, q ) )

export const	//	2D
Angle = ( [ ph, pv ], [ qh, qv ] ) => Math.atan2(
	ph * qv - pv * qh
,	ph * qh + pv * qv
)

export const	//	2D
IntersectionV = ( [ [ px, py ], [ ph, pv ] ], [ [ qx, qy ], [ qh, qv ] ] ) => {
	const deno = ph * qv - pv * qh
	if ( deno === 0 ) return null	//	Parallel
	const t0 = ( ( qv * qx - qh * qy ) - ( qv * px - qh * py ) ) / deno
	const t1 = ( ( pv * qx - ph * qy ) - ( pv * px - ph * py ) ) / deno
	return 0 < t0 && t0 < 1 && 0 < t1 && t1 < 1
	?	Add( [ px, py ], Mul( [ ph, pv ], t0 ) )
	:	null
}

export const	//	2D
Intersection = ( [ sP, eP ], [ sQ, eQ ] ) => IntersectionV( [ sP, Vec( sP, eP ) ], [ sQ, Vec( sQ, eQ ) ] )

//	https://manabitimes.jp/math/857
export const	//	2D
PerpendicularLength2V = ( [ ph, pv ], [ qh, qv ] ) => {
	const deno = ph * ph + pv * pv
	if ( deno == 0 ) return null
	if ( ph < 0 ) {
		ph = -ph
		qh = -qh
	}
	if ( pv < 0 ) {
		pv = -pv
		qv = -qv
	}
	const _ = ( pv * qv + ph * qh ) / deno
	const X = ph * _
	if ( X < 0 || ph < X ) return null
	const Y = pv * _
	if ( Y < 0 || pv < Y ) return null
	const num = ph * qv - pv * qh
	return num * num / deno
}

export const	//	2D
PerpendicularLength2 = ( o, p, q ) => PerpendicularLength2V( Vec( o, p ), Vec( o, q ) )

export const	//	2D
BBox = $ => $.slice( 1 ).reduce(
	( $, _ ) => (
		_[ 0 ] < $[ 0 ] && ( $[ 0 ] = _[ 0 ] )
	,	_[ 1 ] < $[ 1 ] && ( $[ 1 ] = _[ 1 ] )
	,	_[ 0 ] > $[ 2 ] && ( $[ 2 ] = _[ 0 ] )
	,	_[ 1 ] > $[ 3 ] && ( $[ 3 ] = _[ 1 ] )
	,	$
	)
,	[ ...$[ 0 ], ...$[ 0 ] ]
)

export const	//	2D
BBoxOr = ( p, q ) => [
	p[ 0 ] < q[ 0 ] ? p[ 0 ] : q[ 0 ]
,	p[ 1 ] < q[ 1 ] ? p[ 1 ] : q[ 1 ]
,	p[ 2 ] > q[ 2 ] ? p[ 2 ] : q[ 2 ]
,	p[ 3 ] > q[ 3 ] ? p[ 3 ] : q[ 3 ]
]

export const	//	2D
BBoxAnd = ( p, q ) => q[ 0 ] < p[ 2 ] && p[ 0 ] < q[ 2 ] && q[ 1 ] < p[ 3 ] && p[ 1 ] < q[ 3 ]

export const	//	2D
IntersectingPoints = ( p, q ) => {
	if ( BBoxAnd( BBox( p ), BBox( q ) ) ) {
		const $ = []
		let prevP = p[ 0 ]
		p.slice( 1 ).forEach(
			P => {
				let prevQ = q[ 0 ]
				q.slice( 1 ).forEach(
					Q => {
						const _ = Intersection( [ prevP, P ], [ prevQ, Q ] )
						_ && $.push( _ )
						prevQ = Q
					}
				)
				prevP = P
			}
		)
		return $.reduce( ( $, _ ) => ( !$.some( $ => EQ( $, _ ) ) && $.push( _ ), $ ), [] )
	} else {
		return []
	}
}

export const	//	2D
Transform2D = ( $, _ ) => [
	$[ 0 ] * _[ 0 ], $[ 1 ] * _[ 1 ] + _[ 2 ]
,	$[ 0 ] * _[ 3 ], $[ 1 ] * _[ 4 ] + _[ 5 ]
]

export const	//	nD
DivideBezier = ( [ s, p, q, e ], t ) => {
	const u = 1 - t
	return s.map(
		( $, _ ) => {
			const	sp	= ( s[ _ ] * u + p[ _ ] * t )
			const	pq	= ( p[ _ ] * u + q[ _ ] * t )
			const	qe	= ( q[ _ ] * u + e[ _ ] * t )
			const	spq	= ( sp * u + pq * t )
			const	pqe	= ( pq * u + qe * t )
			return	[
				sp
			,	spq
			,	spq * u + pqe * t
			, 	pqe
			,	qe
			]
		}
	).reduce(
		( $, _ ) => ( $.forEach( ( $, i ) => $.push( _[ i ] ) ), $ )
	,	[ [], [], [], [], [] ]
	)
}

export const	//	nD
Div2Bezier = ( [ s, p, q, e ] ) => s.map(
	( $, _ ) => {
		const	sp	= ( s[ _ ] + p[ _ ] ) / 2
		const	pq	= ( p[ _ ] + q[ _ ] ) / 2
		const	qe	= ( q[ _ ] + e[ _ ] ) / 2
		const	spq	= ( sp + pq ) / 2
		const	pqe	= ( pq + qe ) / 2
		return	[
			sp
		,	spq
		,	( spq + pqe ) / 2
		, 	pqe
		,	qe
		]
	}
).reduce(
	( $, _ ) => ( $.forEach( ( $, i ) => $.push( _[ i ] ) ), $ )
,	[ [], [], [], [], [] ]
)

export const	//	2D
FlattenBezier = ( [ s, p, q, e ], d2 ) => {
	const vE = Vec( s, e )
	const deno2 = vE[ 0 ] * vE[ 0 ] + vE[ 1 ] * vE[ 1 ]

	const vP = Vec( s, p )
	const vQ = Vec( s, q )
	const numP = vE[ 0 ] * vP[ 1 ] - vE[ 1 ] * vP[ 0 ]
	const numQ = vE[ 0 ] * vQ[ 1 ] - vE[ 1 ] * vQ[ 0 ]
	const distP2 = numP * numP / deno2
	const distQ2 = numQ * numQ / deno2
	if ( Math.max( distP2, distQ2 ) * ( numP * numQ < 0 ? 4 / 9 : 3 / 4 ) < d2 ) { 
		return [ e ]
	} else {
		const _ = Div2Bezier( [ s, p, q, e ] )
		return [
			...FlattenBezier( [ s, _[ 0 ], _[ 1 ], _[ 2 ] ], d2 )
		,	...FlattenBezier( [ _[ 2 ], _[ 3 ], _[ 4 ], e ], d2 )
		]
	}
}

const
_LinePixelsQ1st = ( h, v ) => {
	if ( h <= 1 && v <= 1 ) return []
	if ( !h )		return Array.from( { length: v - 1 }, ( $, _ ) => [ 0		, _ + 1	] )
	if ( !v )		return Array.from( { length: h - 1 }, ( $, _ ) => [ _ + 1	, 0		] )
	if ( h === v )	return Array.from( { length: h - 1 }, ( $, _ ) => [ _ + 1	, _ + 1	] )
	const	bh = 2 * h
	const	bv = 2 * v
	const	$ = []
	if ( h > v ) {
		let		E = -h 
		let		y = 0
		for ( let x = 1; x < h; x++ ) {
			E += bv
			E >= 0 && ( y++, E -= bh )
			$.push( [ x, y ] )
		}
	} else {
		let		E = -v 
		let		x = 0
		for ( let y = 1; y < v; y++ ) {
			E += bh
			E >= 0 && ( x++, E -= bv )
			$.push( [ x, y ] )
		}
	}
	return $
}
export const
LinePixelsV = ( [ x, y ], [ h, v ] ) => h < 0
?	v < 0
	?	_LinePixelsQ1st( -h, -v ).map( $ => [ x - $[ 0 ], y - $[ 1 ] ] )
	:	_LinePixelsQ1st( -h,  v ).map( $ => [ x - $[ 0 ], y + $[ 1 ] ] )
:	v < 0
	?	_LinePixelsQ1st(  h, -v ).map( $ => [ x + $[ 0 ], y - $[ 1 ] ] )
	:	_LinePixelsQ1st(  h,  v ).map( $ => [ x + $[ 0 ], y + $[ 1 ] ] )

export const
LinePixels = ( [ s, e ] ) => LinePixelsV( s, Vec( s, e ) )

export const	//	nD
BezierPixels = ( [ s, p, q, e ] ) => {

	const $ = Div2Bezier( [ s, p, q, e ] )
	const rM = Round( $[ 2 ] )
	const rS = Round( s )
	const rE = Round( e )

	const eqS = EQ( rS, rM )
	const eqE = EQ( rE, rM )
	if ( eqS && eqE ) return []

	const nearS = Near( rS, rM )
	const nearE = Near( rE, rM )

	if ( ( eqS && nearE ) || ( nearS && eqE ) ) return []

	if ( nearS && nearE ) return [ rM ]

	if ( nearS ) return [ rM, ...BezierPixels( [ $[ 2 ], $[ 3 ], $[ 4 ], e ] ) ]
	if ( nearE ) return [ ...BezierPixels( [ s, $[ 0 ], $[ 1 ], $[ 2 ] ] ), rM ]

	return [
		...BezierPixels( [ s, $[ 0 ], $[ 1 ], $[ 2 ] ] )
	,	rM
	,	...BezierPixels( [ $[ 2 ], $[ 3 ], $[ 4 ], e ] )
	]
}

export const
CrossingPointsByPixels = ( p, q ) => {

	const $ = []

	const
	indexPairs = []
	for ( let _P = 0; _P < p.length; _P++ ) {
		for ( let _Q = 0; _Q < q.length; _Q++ ) {
			if ( EQ( p[ _P ], q[ _Q ] ) ) {
				indexPairs.push( [ _P, _Q ] )
				$.push( p[ _P ] )
			}
		}
	}

	for ( let _P = 1; _P < p.length; _P++ ) {
		for ( let _Q = 1; _Q < q.length; _Q++ ) {
			if (
				indexPairs.some( $ => $[ 0 ] === _P && $[ 1 ] === _Q ) 
			||	indexPairs.some( $ => $[ 0 ] === _P - 1 && $[ 1 ] === _Q - 1 ) 
			) continue

			const P = p[ _P ]
			const Q = q[ _Q ]
			const prevP = p[ _P - 1 ]
			const prevQ = q[ _Q - 1 ]

			const distP = P[ 0 ] - prevP[ 0 ] + P[ 1 ] - prevP[ 1 ]
			if ( distP === 1 || distP === -1 ) continue

			const distQ = Q[ 0 ] - prevQ[ 0 ] + Q[ 1 ] - prevQ[ 1 ]
			if ( distQ === 1 || distQ === -1 ) continue

			const x2 = P[ 0 ] + prevP[ 0 ]
			const y2 = P[ 1 ] + prevP[ 1 ]
			if ( x2 === Q[ 0 ] + prevQ[ 0 ] && y2 === Q[ 1 ] + prevQ[ 1 ] ) $.push( [ x2 / 2, y2 / 2 ] )
		}
	}
	return $
}

export const	//	nD
FindT = ( HIT, [ s, p, q, e ], tS = 0, tE = 1 ) => {	//	HIT must be [ int ]
	const $ = Div2Bezier( [ s, p, q, e ] )

	const rM = Round( $[ 2 ] )

	if ( EQ( HIT, rM ) ) return ( tS + tE ) / 2

	const rS = Round( s )
	const rE = Round( e )

	const nearS = Near( rS, rM )
	const nearE = Near( rE, rM )

	if ( nearS && nearE ) return null

	const _ = FindT( HIT, [ s, $[ 0 ], $[ 1 ], $[ 2 ] ], tS, ( tS + tE ) / 2 )
	return _
	?	_
	:	FindT( HIT, [ $[ 2 ], $[ 3 ], $[ 4 ], e ], ( tS + tE ) / 2, tE )
}

/*	Points: [ s, p, q, e ] 
	BezierDiff: t => {	const u = 1 - t
			u^3s
		+	3u^2tp
		+	3ut^2q
		+	t^3e
		-	$
	}
	A	: u^3s
	B	: 3u^2t
	C	: 3ut^2
	D	: t^3e
	E	: -$

	BezierDiff			: A + Bp + Cq + D + E

	BezierDiff^2		: A^2 + B^2p^2 + C^2q^2 + D^2 + E^2 + 2ABp + 2ACq + 2AD + 2AE + 2BCpq + 2BDp + 2BEp + 2CDq + 2CEq + 2DE
	(BezierDiff^2)'/p/2	: B^2p + BCq + AB + BD + BE
	(BezierDiff^2)'/q/2	: BCp + C^2q + AC + CD + CE

	(BezierDiff^2)'/p/2	: 9u^4t^2p + 9u^3t^3q + 3u^5ts + 3u^2t^4e - 3u^2to
						: (3u^2t)( 3u^2tp + 3ut^2q + u^3s + t^3e - o )

	(BezierDiff^2)'/q/2	: 9u^3t^3p + 9u^2t^4q + 3u^4t^2s + 3ut^5e - 3ut^2o
						: (3ut^2)( 3u^2tp + 3ut^2q + u^3s + t^3e - o )
*/
export const
FitBezier1D = $ => {
	const l = $.length - 1
	const s = $[ 0 ]
	const e = $[ l ]

	let	pp = 0, pq = 0, qp = 0, qq = 0, p = 0, q = 0
	for ( let _ = 1; _ < l; _++ ) { //	Bypass first and last element, because 't' or 'u' will be zero.
		const t = _ / l
		const u = 1 - t
		const P = 3 * u * u * t
		const Q = 3 * u * t * t
		pp += P * P
		pq += P * Q
		qp += Q * P
		qq += Q * Q
		const PQ = $[ _ ] - u * u * u * s - t * t * t * e
		p += P * PQ
		q += Q * PQ
	}

	const dm = pp * qq - pq * qp

	return [
		+ p * qq / dm - q * pq / dm
	,	- p * qp / dm + q * pp / dm
	]
}

export const
FitBezier = $ => {
	const pqs = $[ 0 ].map( ( __, _ ) => FitBezier1D( $.map( $ => $[ _ ] ) ) )
	return [
		pqs.map( $ => $[ 0 ] )
	,	pqs.map( $ => $[ 1 ] )
	]
}
