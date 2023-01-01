export const
CF				= ( -24 + Math.sqrt( 24 * 24 + 64 * 9 ) ) / 18	//	Curve factor	: 0.552285

////////////////////////////////////////////////////////////////	Multidimensional

//v	Int
export const
EQ				= ( p, q ) => p.every( ( $, _ ) => $ === q[ _ ] )

export const
Next			= ( p, q ) => Abs( Sub( p, q ) ).reduce( ( $, _ ) => $ + _, 0 ) <= 1

export const
Near			= ( p, q ) => p.every( ( $, _ ) => Math.abs( $ - q[ _ ] ) <= 1 )
//^	Int

export const
Round			= _ => _.map( _ => Math.round( _ ) )

export const
Abs				= _ => _.map( _ => Math.abs( _ ) )

export const
Add				= ( p, q ) => p.map( ( $, _ ) => $ + q[ _ ] )

export const
Sub				= ( p, q ) => p.map( ( $, _ ) => $ - q[ _ ] )

export const
Vec				= ( p, q ) => p.map( ( $, _ ) => q[ _ ] - $ )

export const
Mul				= ( $, _ ) => $.map( $ => $ * _ )

export const
Div				= ( $, _ ) => $.map( $ => $ / _ )

export const
Mid				= ( p, q ) => p.map( ( $, _ ) => ( $ + q[ _ ] ) / 2 )

export const
Dot				= ( p, q ) => p.reduce( ( $$, $, _ ) => $$ + $ * q[ _ ], 0 )

export const
Norm			= _ => Math.sqrt( Dot( _, _ ) )

export const
Transpose		= _ => _[ 0 ].map( ( __, d ) => _.map( _ => _[ d ] ) )

export const
BBox			= ( _, ...$ ) => $.reduce(
	( $$, $ ) => $.map(
		( $, _ ) => (
			_ = $$[ _ ]
		,	[	$ < _[ 0 ] ? $ : _[ 0 ]
			,	$ > _[ 1 ] ? $ : _[ 1 ]
			]
		)
	)
,	_.map( _ => [ _, _ ] )
)

export const
BBoxOr			= ( _, ...$ ) => $.reduce(
	( $$, $ ) => $.map(
		( $, _ ) => (
			_ = $$[ _ ]
		,	[	$[ 0 ] < _[ 0 ] ? $[ 0 ] : _[ 0 ]
			,	$[ 1 ] > _[ 1 ] ? $[ 1 ] : _[ 1 ]
			]
		)
	)
,	_
)

//	The return value of this function must be checked to ensure that all dimensions are non void.
export const
BBoxAnd			= ( _, ...$ ) => $.reduce(
	( $$, $ ) => $.map(
		( $, _ ) => (
			_ = $$[ _ ]
		,	$[ 1 ] > _[ 0 ] && _[ 1 ] > $[ 0 ]
			?	[	$[ 0 ] < _[ 0 ] ? _[ 0 ] : $[ 0 ]
				,	$[ 1 ] > _[ 1 ] ? _[ 1 ] : $[ 1 ]
				]
			:	void 0
		)
	)
)

////////////////////////////////////////////////////////////////	3D

export const	//	3D
Cross3D = ( [ px, py, pz ], [ qx, qy, qz ] ) => [
	py * qz - pz * qy
,	pz * qx - px * qz
,	px * qy - py * qx
]

export const	//	3D
FormedAngle = ( p, q ) => Math.atan2( Norm( Cross3D( p, q ) ), Dot( p, q ) )

////////////////////////////////////////////////////////////////	2D

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
IntersectingPoints = ( p, q ) => {
	if ( BBoxAnd( BBox( p ), BBox( q ) ).every( _ => _ ) ) {
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
		return $
	} else {
		return []
	}
}

export const	//	2D
Transform2D = ( $, _ ) => [
	$[ 0 ] * _[ 0 ], $[ 1 ] * _[ 1 ] + _[ 2 ]
,	$[ 0 ] * _[ 3 ], $[ 1 ] * _[ 4 ] + _[ 5 ]
]

////////////////////////////////////////////////////////////////	Pixels(Obsolute) 2D

/*
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
*/

////////////////////////////////////////////////////////////////	Grids 2D

const	//	!!DESTRUCTIVE!!
OptGrids = ( s, $, e ) => {
	{	let next	= e
		let _		= $.length
		while ( _-- ) {
			const prev = $[ _ ]
			EQ( prev, next ) && $.splice( _, 1 )
			next = prev
		}
		$.length && EQ( s, $[ 0 ] ) && $.splice( 0, 1 )
	}
	if ( $.length ) {
		let next = Sub( $.at( -1 ), e )
		let _ = $.length
		while ( --_ ) {
			const prev = Sub( $[ _ - 1 ], $[ _ ] )
			if ( ( prev[ 0 ] === 0 && next[ 1 ] === 0 ) || ( prev[ 1 ] === 0 && next[ 0 ] === 0 ) ) $.splice( _, 1 )
			next = prev
		}
		const prev = Sub( s, $[ 0 ] )
		if ( ( prev[ 0 ] === 0 && next[ 1 ] === 0 ) || ( prev[ 1 ] === 0 && next[ 0 ] === 0 ) ) $.splice( _, 1 )
	}
	return $
}

const
_LineGrids = ( s, e ) => {
	const rS	= Round( s )
	const rE	= Round( e )
	if ( Near( rS, rE ) ) return []

	const $		= Mid( s, e )
	const rM	= Round( $ )

	return [
		..._LineGrids( s, $ )
	,	rM
	,	..._LineGrids( $, e )
	]
}
export const
LineGrids = ( s, e ) => OptGrids( s, _LineGrids( s, e ), e )

const
_QuadGrids = ( s, c, e ) => {
	const rS	= Round( s )
	const rE	= Round( e )

	const sc	= Mid( s, c )
	const ce	= Mid( c, e )
	const $		= Mid( sc, ce )
	const rM	= Round( $ )

	return Near( rS, rM ) && Near( rM, rE )
	?	Near( rS, rE ) ? [] : [ rM ]
	:	[	..._QuadGrids( s, sc, $ )
		,	rM
		,	..._QuadGrids( $, ce, e )
		]
}
export const
QuadGrids = ( s, c, e ) => OptGrids( s, _QuadGrids( s, c, e ), e )

const
_CubeGrids = ( s, p, q, e ) => {
	const rS	= Round( s )
	const rE	= Round( e )

	const sp	= Mid( s, p )
	const qe	= Mid( q, e )
	const pq	= Mid( p, q )
	const spq	= Mid( sp, pq )
	const pqe	= Mid( pq, qe )
	const $		= Mid( spq, pqe )

	const rM	= Round( $ )

	return Near( rS, rM ) && Near( rM, rE )
	?	Near( rS, rE ) ? [] : [ rM ]
	:	[	..._CubeGrids( s, sp, spq, $ )
		,	rM
		,	..._CubeGrids( $, pqe, qe, e )
		]
}
export const
CubeGrids = ( s, p, q, e ) => OptGrids( s, _CubeGrids( s, p, q, e ), e )

export const
IntersectingGrids = ( p, q ) => {

	const $ = []

	for ( let P of p ) for ( let Q of q ) EQ( P, Q ) && $.push( P )

	//	FINDING DIAGONAL CROSSING
	let prevP = p[ 0 ]
	for ( let _P = 1; _P < p.length; _P++ ) {
		const P = p[ _P ]
		if ( prevP[ 0 ] !== P[ 0 ] && prevP[ 1 ] !== P[ 1 ] ) {
			let prevQ = q[ 0 ]
			for ( let _Q = 1; _Q < q.length; _Q++ ) {
				const Q = q[ _Q ]
				if ( prevQ[ 0 ] !== Q[ 0 ] && prevQ[ 1 ] !== Q[ 1 ] ) {
					Next( prevP, prevQ ) && Next( prevP, Q ) && Next( P, prevQ ) && Next( P, Q ) && $.push( Mid( prevP, P ) )
				}
				prevQ = Q
			}
		}
		prevP = P
	}
	return $
}

export const	//	2D
IsLineGrids = ( s, _, e ) => {
	const pq = Vec( s, e )
	return _.every( r => Math.abs( PerpendicularLength2V( pq, Vec( s, r ) ) ) < 1 )
}

////////////////////////////////////////////////////////////////	Bezier Multidimensional

export const	//	nD
DivideCubeBezier = ( [ s, p, q, e ], t ) => {
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
			,	pqe
			,	qe
			]
		}
	).reduce(
		( $, _ ) => ( $.forEach( ( $, i ) => $.push( _[ i ] ) ), $ )
	,	[ [], [], [], [], [] ]
	)
}

export const	//	nD
DivideQuadBezier = ( [ s, c, e ], t ) => {
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
Div2CubeBezier = ( [ s, p, q, e ] ) => s.map(
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
		,	pqe
		,	qe
		]
	}
).reduce(
	( $, _ ) => ( $.forEach( ( $, i ) => $.push( _[ i ] ) ), $ )
,	[ [], [], [], [], [] ]
)

export const	//	nD
Div2QuadBezier = ( [ s, c, e ] ) => s.map(
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

export const	//	nD
FindCubeBezierT = ( HIT, [ s, p, q, e ], tS = 0, tE = 1 ) => {	//	HIT must be [ int ]
	const $ = Div2CubeBezier( [ s, p, q, e ] )

	const rM = Round( $[ 2 ] )

//console.log( HIT, rM, Round( s ), Round( e ) )
	if ( Near( HIT, rM ) ) return ( tS + tE ) / 2

	if ( Near( Round( s ), Round( e ) ) ) return null

	const _ = FindCubeBezierT( HIT, [ s, $[ 0 ], $[ 1 ], $[ 2 ] ], tS, ( tS + tE ) / 2 )
	return _
	?	_
	:	FindCubeBezierT( HIT, [ $[ 2 ], $[ 3 ], $[ 4 ], e ], ( tS + tE ) / 2, tE )
}

export const	//	nD
FindQuadBezierT = ( HIT, [ s, c, e ], tS = 0, tE = 1 ) => {	//	HIT must be [ int ]
	const $ = Div2QuadBezier( [ s, c, e ] )

	const rM = Round( $[ 1 ] )

	if ( Near( HIT, rM ) ) return ( tS + tE ) / 2

	if ( Near( Round( s ), Round( e ) ) ) return null

	const _ = FindQuadBezierT( HIT, [ s, $[ 0 ], $[ 1 ] ], tS, ( tS + tE ) / 2 )
	return _
	?	_
	:	FindQuadBezierT( HIT, [ $[ 1 ], $[ 2 ], e ], ( tS + tE ) / 2, tE )
}


/*	Points: [ s, p, q, e ] 
	CubeBezierDiff: t => {	const u = 1 - t
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
	CubeBezierDiff			: A + Bp + Cq + D + E

	CubeBezierDiff^2		: A^2 + B^2p^2 + C^2q^2 + D^2 + E^2 + 2ABp + 2ACq + 2AD + 2AE + 2BCpq + 2BDp + 2BEp + 2CDq + 2CEq + 2DE
	(CubeBezierDiff^2)'/p/2	: B^2p + BCq + AB + BD + BE
	(CubeBezierDiff^2)'/q/2	: BCp + C^2q + AC + CD + CE

	(CubeBezierDiff^2)'/p/2	: 9u^4t^2p + 9u^3t^3q + 3u^5ts + 3u^2t^4e - 3u^2t$
						: (3u^2t)( 3u^2tp + 3ut^2q + u^3s + t^3e - $ )

	(CubeBezierDiff^2)'/q/2	: 9u^3t^3p + 9u^2t^4q + 3u^4t^2s + 3ut^5e - 3ut^2$
						: (3ut^2)( 3u^2tp + 3ut^2q + u^3s + t^3e - $ )
*/
const
FitCubeBezier1D = _ => {
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
FitCubeBezier = _ => Transpose( Transpose( _ ).map( _ => FitCubeBezier1D( _ ) ) )

/*	Points: [ s, ?, e ] 
	QuadBezierDiff: t => {	const u = 1 - t
			u^2s
		+	2ut?
		+	t^2e
		-	$		// 実際の値
	}

	A	: u^2s
	B	: 2ut
	C	: t^2e
	D	: -$
	QuadBezierDiff			: A + B? + C + D

	QuadBezierDiff^2		: A^2 + B^2?^2 + C^2 + D^2 + 2AB? + 2AC + 2AD + 2BC? + 2BD? + 2CD
	(QuadBezierDiff^2)'/2	: B^2? + AB + BC + BD
						: 4u^2t^2? + 2u^3ts + 2ut^3e - 2ut$
						: (2ut)( 2ut? + u^2s + t^2e - $ )
	2ut? + u^2s + t^2e - $ = 0
	2? = ( $ / ut ) - ( us / t ) - ( te / u )

*/

const
FitQuadBezier1D = _ => {
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
FitQuadBezier = _ => Transpose( _ ).map( _ => FitQuadBezier1D( _ ) )

////////////////////////////////////////////////////////////////	Bezier 2D

export const
FlattenCubeBezier = ( [ s, p, q, e ], d2 ) => {
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
		const _ = Div2CubeBezier( [ s, p, q, e ] )
		return [
			...FlattenCubeBezier( [ s, _[ 0 ], _[ 1 ], _[ 2 ] ], d2 )
		,	...FlattenCubeBezier( [ _[ 2 ], _[ 3 ], _[ 4 ], e ], d2 )
		]
	}
}

export const
FlattenQuadBezier = ( [ s, c, e ], d2 ) => {
	if ( PerpendicularLength2( s, e, c ) / 2 < d2 ) {
		return [ e ]
	} else {
		const _ = Div2QuadBezier( [ s, c, e ] )
		return [
			...FlattenQuadBezier( [ s, _[ 0 ], _[ 1 ] ], d2 )
		,	...FlattenQuadBezier( [ _[ 1 ], _[ 2 ], e ], d2 )
		]
	}
}

