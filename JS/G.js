export const
CF	= ( -24 + Math.sqrt( 24 * 24 + 64 * 9 ) ) / 18	//	Curve factor	: 0.552285

export const	//	Int
EQ = ( p, q ) => p.every( ( $, _ ) => $ === q[ _ ] )

export const
Round = $ => $.map( $ => Math.round( $ ) )

export const
Abs = _ => _.map( _ => Math.abs( _ ) )

export const	//	Int
Next = ( p, q ) => Abs( Sub( p, q ) ).reduce( ( $, _ ) => $ + _, 0 ) <= 1

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
	const deno2 = ph * ph + pv * pv
	if ( deno2 == 0 ) return null
	if ( ph < 0 ) {
		ph = -ph
		qh = -qh
	}
	if ( pv < 0 ) {
		pv = -pv
		qv = -qv
	}
	const _ = ( pv * qv + ph * qh ) / deno2
	const X = ph * _
	if ( X < 0 || ph < X ) return null
	const Y = pv * _
	if ( Y < 0 || pv < Y ) return null
	const num = ph * qv - pv * qh
	return num * num / deno2
}

export const	//	2D
PerpendicularLength2 = ( o, p, q ) => PerpendicularLength2V( Vec( o, p ), Vec( o, q ) )

export const	//	2D
IsLinePixels = _ => {
	const lastI = _.length - 1
	const pq = Vec( _[ 0 ], _[ lastI ] )
	return _.slice( 1, lastI ).every( r => Math.abs( PerpendicularLength2V( pq, Vec( _[ 0 ], r ) ) ) < 1 )
}

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
DivideConic = ( [ s, c, e ], t ) => {
	const u = 1 - t
	return s.map(
		( $, _ ) => {
			const	sc	= ( s[ _ ] * u + c[ _ ] * t )
			const	ce	= ( c[ _ ] * u + e[ _ ] * t )
			return	[
				sc
			,	sc * u + ce * t
			,	ce
			]
		}
	).reduce(
		( $, _ ) => ( $.forEach( ( $, i ) => $.push( _[ i ] ) ), $ )
	,	[ [], [], [] ]
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

export const	//	nD
Div2Conic = ( [ s, c, e ] ) => s.map(
	( $, _ ) => {
		const	sc	= ( s[ _ ] + c[ _ ] ) / 2
		const	ce	= ( c[ _ ] + e[ _ ] ) / 2
		return	[
			sc
		,	( sc + ce ) / 2
		,	ce
		]
	}
).reduce(
	( $, _ ) => ( $.forEach( ( $, i ) => $.push( _[ i ] ) ), $ )
,	[ [], [], [] ]
)

export const	//	2D
FlattenBezier = ( [ s, p, q, e ], d2 ) => {
	const vE = Vec( s, e )
	const deno2 = vE[ 0 ] * vE[ 0 ] + vE[ 1 ] * vE[ 1 ]

	const vP = Vec( s, p )
	const vQ = Vec( s, q )
	const numP = vE[ 0 ] * vP[ 1 ] - vE[ 1 ] * vP[ 0 ]
	const numQ = vE[ 0 ] * vQ[ 1 ] - vE[ 1 ] * vQ[ 0 ]
	const d2P = numP * numP / deno2
	const d2Q = numQ * numQ / deno2
	if ( Math.max( d2P, d2Q ) * ( numP * numQ < 0 ? 4 / 9 : 3 / 4 ) < d2 ) { 
		return [ e ]
	} else {
		const _ = Div2Bezier( [ s, p, q, e ] )
		return [
			...FlattenBezier( [ s, _[ 0 ], _[ 1 ], _[ 2 ] ], d2 )
		,	...FlattenBezier( [ _[ 2 ], _[ 3 ], _[ 4 ], e ], d2 )
		]
	}
}

export const	//	2D
FlattenConic = ( [ s, c, e ], d2 ) => {
	if ( PerpendicularLength2( s, e, c ) / 2 < d2 ) {
		return [ e ]
	} else {
		const _ = Div2Conic( [ s, c, e ] )
		return [
			...FlattenConic( [ s, _[ 0 ], _[ 1 ] ], d2 )
		,	...FlattenConic( [ _[ 1 ], _[ 2 ], e ], d2 )
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

export const	//	nD
ConicPixels = ( [ s, c, e ] ) => {

	const $ = Div2Conic( [ s, c, e ] )
	const rM = Round( $[ 1 ] )
	const rS = Round( s )
	const rE = Round( e )

	const eqS = EQ( rS, rM )
	const eqE = EQ( rE, rM )
	if ( eqS && eqE ) return []

	const nearS = Near( rS, rM )
	const nearE = Near( rE, rM )

	if ( ( eqS && nearE ) || ( nearS && eqE ) ) return []

	if ( nearS && nearE ) return [ rM ]

	if ( nearS ) return [ rM, ...ConicPixels( [ $[ 1 ], $[ 2 ], e ] ) ]
	if ( nearE ) return [ ...ConicPixels( [ s, $[ 0 ], $[ 1 ] ] ), rM ]

	return [
		...ConicPixels( [ s, $[ 0 ], $[ 1 ] ] )
	,	rM
	,	...ConicPixels( [ $[ 1 ], $[ 2 ], e ] )
	]
}

export const
PointsOfIntersectionByPixels = ( p, q ) => {

	const $ = []

	for ( let P of p ) for ( let Q of q ) EQ( P, Q ) && $.push( P )

	let prevP = p[ 0 ]
	for ( let _P = 1; _P < p.length; _P++ ) {
		const P = p[ _P ]
		if ( prevP[ 0 ] !== P[ 0 ] && prevP[ 1 ] !== P[ 1 ] ) {
			let prevQ = q[ 0 ]
			for ( let _Q = 1; _Q < q.length; _Q++ ) {
				const Q = q[ _Q ]
				if ( prevQ[ 0 ] !== Q[ 0 ] && prevQ[ 1 ] !== Q[ 1 ] ) {
					//	DIAGONAL CROSSING
					Next( prevP, prevQ ) && Next( prevP, Q ) && Next( P, prevQ ) && Next( P, Q ) && $.push(
						[ ( prevP[ 0 ] + P[ 0 ] ) / 2, ( prevP[ 1 ] + P[ 1 ] ) / 2 ]
					)
				}
				prevQ = Q
			}
		}
		prevP = P
	}
	return $
}

export const	//	nD
FindBezierT = ( HIT, [ s, p, q, e ], tS = 0, tE = 1 ) => {	//	HIT must be [ int ]
	const $ = Div2Bezier( [ s, p, q, e ] )

	const rM = Round( $[ 2 ] )

	if ( EQ( HIT, rM ) ) return ( tS + tE ) / 2

	const rS = Round( s )
	const rE = Round( e )

	const nearS = Near( rS, rM )
	const nearE = Near( rE, rM )

	if ( nearS && nearE ) return null

	const _ = FindBezierT( HIT, [ s, $[ 0 ], $[ 1 ], $[ 2 ] ], tS, ( tS + tE ) / 2 )
	return _
	?	_
	:	FindBezierT( HIT, [ $[ 2 ], $[ 3 ], $[ 4 ], e ], ( tS + tE ) / 2, tE )
}

export const	//	nD
FindConicT = ( HIT, [ s, c, e ], tS = 0, tE = 1 ) => {	//	HIT must be [ int ]
	const $ = Div2Conic( [ s, c, e ] )

	const rM = Round( $[ 1 ] )

	if ( EQ( HIT, rM ) ) return ( tS + tE ) / 2

	const rS = Round( s )
	const rE = Round( e )

	const nearS = Near( rS, rM )
	const nearE = Near( rE, rM )

	if ( nearS && nearE ) return null

	const _ = FindConicT( HIT, [ s, $[ 0 ], $[ 1 ] ], tS, ( tS + tE ) / 2 )
	return _
	?	_
	:	FindConicT( HIT, [ $[ 1 ], $[ 2 ], e ], ( tS + tE ) / 2, tE )
}

const
Transpose = _ => _[ 0 ].map( ( __, d ) => _.map( _ => _[ d ] ) )


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

	(BezierDiff^2)'/p/2	: 9u^4t^2p + 9u^3t^3q + 3u^5ts + 3u^2t^4e - 3u^2t$
						: (3u^2t)( 3u^2tp + 3ut^2q + u^3s + t^3e - $ )

	(BezierDiff^2)'/q/2	: 9u^3t^3p + 9u^2t^4q + 3u^4t^2s + 3ut^5e - 3ut^2$
						: (3ut^2)( 3u^2tp + 3ut^2q + u^3s + t^3e - $ )
*/
export const
FitBezier1D = _ => {
	const l = _.length - 1
	const s = _[ 0 ]
	const e = _[ l ]

	let	pp = 0, pq = 0, qp = 0, qq = 0, p = 0, q = 0
	for ( let $ = 1; $ < l; $++ ) {	//	Bypass first and last element, because 't' or 'u' will be zero.
		const t = $ / l
		const u = 1 - t
		const P = 3 * u * u * t
		const Q = 3 * u * t * t
		pp += P * P
		qq += Q * Q
		const PQ = P * Q
		pq += PQ
		qp += PQ
		const R = _[ $ ] - u * u * u * s - t * t * t * e
		p += P * R
		q += Q * R
	}

	const dm = pp * qq - pq * qp

	return [
		+ p * qq / dm - q * pq / dm
	,	- p * qp / dm + q * pp / dm
	]
}

export const
FitBezier = _ => Transpose( Transpose( _ ).map( _ => FitBezier1D( _ ) ) )

/*	Points: [ s, ?, e ] 
	ConicDiff: t => {	const u = 1 - t
			u^2s
		+	2ut?
		+	t^2e
		-	$		// 実際の値
	}

	A	: u^2s
	B	: 2ut
	C	: t^2e
	D	: -$
	ConicDiff			: A + B? + C + D

	ConnicDiff^2		: A^2 + B^2?^2 + C^2 + D^2 + 2AB? + 2AC + 2AD + 2BC? + 2BD? + 2CD
	(ConnicDiff^2)'/2	: B^2? + AB + BC + BD
						: 4u^2t^2? + 2u^3ts + 2ut^3e - 2ut$
						: (2ut)( 2ut? + u^2s + t^2e - $ )
	2ut? + u^2s + t^2e - $ = 0
	2? = ( $ / ut ) - ( us / t ) - ( te / u )

*/

export const
FitConic1D = _ => {
	const l = _.length - 1
	const s = _[ 0 ]
	const e = _[ l ]

	let C = 0
	let S = 0
	let E = 0
	for ( let $ = 1; $ < l; $++ ) { //	Bypass first and last element, because 't' or 'u' will be zero.
		const t = $ / l
		const u = 1 - t
		C += _[ $ ] / ( u * t )
		S += u * s / t
		E += t * e / u
	}

	return ( C - S - E ) / 2 / ( l - 1 )
}

export const
FitConic = _ => Transpose( _ ).map( _ => FitConic1D( _ ) )

