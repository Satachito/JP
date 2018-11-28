//	Written by Satoru Ogura, Tokyo.
//

import Accelerate

struct
Vector< N: Numeric > {
	let	m	: ArraySlice< N >
	let	n	: Int
	let	s	: Int
	init( _ m: ArraySlice< N > ) {
		self.m = m
		self.n = m.count
		self.s = 1
	}
	init( _ m: ArraySlice< N >, _ n: Int, _ s: Int ) {
		self.m = m
		self.n = n
		self.s = s
	}
	init( _ n: Int, _ initial: N = 0 ) {
		self.m = ArraySlice< N >( repeating: initial, count: n )
		self.n = n
		self.s = 1
	}
	func
	ToArraySlice() -> ArraySlice< N > {
		if s == 1 { return m }
		return ArraySlice( ( 0 ..< Int( n ) ).map { m[ m.startIndex + $0 * s ] } )
	}
	subscript( p: Int ) -> N {
		return m[ m.startIndex + p * s ]
	}
};

func
==< N > ( l: Vector< N >, r: Vector< N > ) -> Bool {
	if l.n != r.n { return false }
	for i in 0 ..< l.n { if l[ i ] != r[ i ] { return false } }
	return true
}
func
!=< N > ( l: Vector< N >, r: Vector< N > ) -> Bool {
	return !( l == r )
}


func	RandomArray	( _ p: Int, _ range: Range			< Int		> ) -> ArraySlice	< Int		> { return ArraySlice( ( 0 ..< p ).map { _ in Int	.random( in: range ) } ) }
func	RandomArray	( _ p: Int, _ range: Range			< Float		> ) -> ArraySlice	< Float		> { return ArraySlice( ( 0 ..< p ).map { _ in Float	.random( in: range ) } ) }
func	RandomArray	( _ p: Int, _ range: Range			< Double	> ) -> ArraySlice	< Double	> { return ArraySlice( ( 0 ..< p ).map { _ in Double.random( in: range ) } ) }
func	RandomArray	( _ p: Int, _ range: ClosedRange	< Int		> ) -> ArraySlice	< Int		> { return ArraySlice( ( 0 ..< p ).map { _ in Int	.random( in: range ) } ) }
func	RandomArray	( _ p: Int, _ range: ClosedRange	< Float		> ) -> ArraySlice	< Float		> { return ArraySlice( ( 0 ..< p ).map { _ in Float	.random( in: range ) } ) }
func	RandomArray	( _ p: Int, _ range: ClosedRange	< Double	> ) -> ArraySlice	< Double	> { return ArraySlice( ( 0 ..< p ).map { _ in Double.random( in: range ) } ) }

func	RandomVector( _ p: Int, _ range: Range			< Int		> ) -> Vector		< Int		> { return Vector< Int		>( RandomArray( p, range ) ) }
func	RandomVector( _ p: Int, _ range: Range			< Float		> ) -> Vector		< Float		> { return Vector< Float	>( RandomArray( p, range ) ) }
func	RandomVector( _ p: Int, _ range: Range			< Double	> ) -> Vector		< Double	> { return Vector< Double	>( RandomArray( p, range ) ) }
func	RandomVector( _ p: Int, _ range: ClosedRange	< Int		> ) -> Vector		< Int		> { return Vector< Int		>( RandomArray( p, range ) ) }
func	RandomVector( _ p: Int, _ range: ClosedRange	< Float		> ) -> Vector		< Float		> { return Vector< Float	>( RandomArray( p, range ) ) }
func	RandomVector( _ p: Int, _ range: ClosedRange	< Double	> ) -> Vector		< Double	> { return Vector< Double	>( RandomArray( p, range ) ) }

func	RampArray	( _ p: Int, _ pInit: Float	= 0, _ pStep: Float		= 1 ) -> ArraySlice	< Float		> { var v = ArraySlice< Float	>( repeating: 0, count: p ); var wInit = pInit;	var	wStep = pStep; vDSP_vramp ( &wInit, &wStep, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p ) ); return v }
func	RampArray	( _ p: Int, _ pInit: Double	= 0, _ pStep: Double	= 1 ) -> ArraySlice	< Double	> { var v = ArraySlice< Double	>( repeating: 0, count: p ); var wInit = pInit;	var	wStep = pStep; vDSP_vrampD( &wInit, &wStep, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p ) ); return v }
func	RampVector	( _ p: Int, _ pInit: Float	= 0, _ pStep: Float		= 1 ) -> Vector		< Float		> { return Vector< Float	>( RampArray( p, pInit, pStep ) ) }
func	RampVector	( _ p: Int, _ pInit: Double	= 0, _ pStep: Double	= 1 ) -> Vector		< Double	> { return Vector< Double	>( RampArray( p, pInit, pStep ) ) }


//	----------------------------------------------------------------

func	Sum			( _ p: ArraySlice< Float	>									) -> Float					{ var v: Float	= 0;														vDSP_sve		( p.withUnsafeBufferPointer { $0.baseAddress! }, 1, &v, vDSP_Length( p.count ) );																												return v }
func	Sum			( _ p: ArraySlice< Double	>									) -> Double					{ var v: Double	= 0;														vDSP_sveD		( p.withUnsafeBufferPointer { $0.baseAddress! }, 1, &v, vDSP_Length( p.count ) );																												return v }
func	Mean		( _ p: ArraySlice< Float	>									) -> Float					{ var v: Float	= 0;														vDSP_meanv		( p.withUnsafeBufferPointer { $0.baseAddress! }, 1, &v, vDSP_Length( p.count ) );																												return v }
func	Mean		( _ p: ArraySlice< Double	>									) -> Double					{ var v: Double	= 0;														vDSP_meanvD		( p.withUnsafeBufferPointer { $0.baseAddress! }, 1, &v, vDSP_Length( p.count ) );																												return v }
func	L2NormQ		( _ p: ArraySlice< Float	>									) -> Float					{ var v: Float	= 0;														vDSP_svesq		( p.withUnsafeBufferPointer { $0.baseAddress! }, 1, &v, vDSP_Length( p.count ) );																												return v }
func	L2NormQ		( _ p: ArraySlice< Double	>									) -> Double					{ var v: Double	= 0;														vDSP_svesqD		( p.withUnsafeBufferPointer { $0.baseAddress! }, 1, &v, vDSP_Length( p.count ) );																												return v }
func	L2Norm		( _ p: ArraySlice< Float	>									) -> Float					{																																																																							return sqrt( L2NormQ( p ) ) }
func	L2Norm		( _ p: ArraySlice< Double	>									) -> Double					{																																																																							return sqrt( L2NormQ( p ) ) }
func	UnitVector	( _ p: ArraySlice< Float	>									) -> ArraySlice< Float	>	{																																																																							return p / L2Norm( p ) }
func	UnitVector	( _ p: ArraySlice< Double	>									) -> ArraySlice< Double	>	{																																																																							return p / L2Norm( p ) }
func	Abs			( _ p: ArraySlice< Float	>									) -> ArraySlice< Float	>	{ var v = ArraySlice< Float	>( repeating: 0, count: p.count );				vDSP_vabs		( p.withUnsafeBufferPointer { $0.baseAddress! }, 1, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p.count ) );															return v }
func	Abs			( _ p: ArraySlice< Double	>									) -> ArraySlice< Double	>	{ var v = ArraySlice< Double>( repeating: 0, count: p.count );				vDSP_vabsD		( p.withUnsafeBufferPointer { $0.baseAddress! }, 1, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p.count ) );															return v }
prefix func	-		( _ p: ArraySlice< Float	>									) -> ArraySlice< Float	>	{ var v = ArraySlice< Float	>( repeating: 0, count: p.count );				vDSP_vneg		( p.withUnsafeBufferPointer { $0.baseAddress! }, 1, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p.count ) );															return v }
prefix func	-		( _ p: ArraySlice< Double	>									) -> ArraySlice< Double	>	{ var v = ArraySlice< Double>( repeating: 0, count: p.count );				vDSP_vnegD		( p.withUnsafeBufferPointer { $0.baseAddress! }, 1, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p.count ) );															return v }
func	DistanceQ	( _ l: ArraySlice< Float	>	, _ r: ArraySlice< Float	>	) -> Float					{ var v: Float	= 0;														vDSP_distancesq	( l.withUnsafeBufferPointer { $0.baseAddress! }, 1, r.withUnsafeBufferPointer { $0.baseAddress! }, 1, &v, vDSP_Length( l.count ) );																return v }
func	DistanceQ	( _ l: ArraySlice< Double	>	, _ r: ArraySlice< Double	>	) -> Double					{ var v: Double	= 0;														vDSP_distancesqD( l.withUnsafeBufferPointer { $0.baseAddress! }, 1, r.withUnsafeBufferPointer { $0.baseAddress! }, 1, &v, vDSP_Length( l.count ) );																return v }
func	Dot			( _ l: ArraySlice< Float	>	, _ r: ArraySlice< Float	>	) -> Float					{ var v: Float	= 0;														vDSP_dotpr		( r.withUnsafeBufferPointer { $0.baseAddress! }, 1, l.withUnsafeBufferPointer { $0.baseAddress! }, 1, &v, vDSP_Length( l.count ) );																return v }
func	Dot			( _ l: ArraySlice< Double	>	, _ r: ArraySlice< Double	>	) -> Double					{ var v: Double	= 0;														vDSP_dotprD		( r.withUnsafeBufferPointer { $0.baseAddress! }, 1, l.withUnsafeBufferPointer { $0.baseAddress! }, 1, &v, vDSP_Length( l.count ) );																return v }
func	+			( _ l: ArraySlice< Float	>	, _ r: ArraySlice< Float	>	) -> ArraySlice< Float	>	{ var v = ArraySlice< Float	>( repeating: 0, count: l.count );				vDSP_vadd		( l.withUnsafeBufferPointer { $0.baseAddress! }, 1, r.withUnsafeBufferPointer { $0.baseAddress! }, 1, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( l.count ) );		return v }
func	+			( _ l: ArraySlice< Double	>	, _ r: ArraySlice< Double	>	) -> ArraySlice< Double	>	{ var v = ArraySlice< Double>( repeating: 0, count: l.count );				vDSP_vaddD		( l.withUnsafeBufferPointer { $0.baseAddress! }, 1, r.withUnsafeBufferPointer { $0.baseAddress! }, 1, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( l.count ) );		return v }
func	-			( _ l: ArraySlice< Float	>	, _ r: ArraySlice< Float	>	) -> ArraySlice< Float	>	{ var v = ArraySlice< Float	>( repeating: 0, count: l.count );				vDSP_vsub		( r.withUnsafeBufferPointer { $0.baseAddress! }, 1, l.withUnsafeBufferPointer { $0.baseAddress! }, 1, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( l.count ) );		return v }
func	-			( _ l: ArraySlice< Double	>	, _ r: ArraySlice< Double	>	) -> ArraySlice< Double	>	{ var v = ArraySlice< Double>( repeating: 0, count: l.count );				vDSP_vsubD		( r.withUnsafeBufferPointer { $0.baseAddress! }, 1, l.withUnsafeBufferPointer { $0.baseAddress! }, 1, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( l.count ) );		return v }
func	*			( _ l: ArraySlice< Float	>	, _ r: ArraySlice< Float	>	) -> ArraySlice< Float	>	{ var v = ArraySlice< Float	>( repeating: 0, count: l.count );				vDSP_vmul		( l.withUnsafeBufferPointer { $0.baseAddress! }, 1, r.withUnsafeBufferPointer { $0.baseAddress! }, 1, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( l.count ) );		return v }
func	*			( _ l: ArraySlice< Double	>	, _ r: ArraySlice< Double	>	) -> ArraySlice< Double	>	{ var v = ArraySlice< Double>( repeating: 0, count: l.count );				vDSP_vmulD		( l.withUnsafeBufferPointer { $0.baseAddress! }, 1, r.withUnsafeBufferPointer { $0.baseAddress! }, 1, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( l.count ) );		return v }
func	/			( _ l: ArraySlice< Float	>	, _ r: ArraySlice< Float	>	) -> ArraySlice< Float	>	{ var v = ArraySlice< Float	>( repeating: 0, count: l.count );				vDSP_vdiv		( r.withUnsafeBufferPointer { $0.baseAddress! }, 1, l.withUnsafeBufferPointer { $0.baseAddress! }, 1, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( l.count ) );		return v }
func	/			( _ l: ArraySlice< Double	>	, _ r: ArraySlice< Double	>	) -> ArraySlice< Double	>	{ var v = ArraySlice< Double>( repeating: 0, count: l.count );				vDSP_vdivD		( r.withUnsafeBufferPointer { $0.baseAddress! }, 1, l.withUnsafeBufferPointer { $0.baseAddress! }, 1, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( l.count ) );		return v }

func	+			( _ p: ArraySlice< Float	>	, _ s: Float					) -> ArraySlice< Float	>	{ var v = ArraySlice< Float	>( repeating: 0, count: p.count ); var w = s;	vDSP_vsadd		( p.withUnsafeBufferPointer { $0.baseAddress! }, 1, &w, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p.count ) );														return v }
func	+			( _ p: ArraySlice< Double	>	, _ s: Double					) -> ArraySlice< Double	>	{ var v = ArraySlice< Double>( repeating: 0, count: p.count ); var w = s;	vDSP_vsaddD		( p.withUnsafeBufferPointer { $0.baseAddress! }, 1, &w, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p.count ) );														return v }
func	-			( _ p: ArraySlice< Float	>	, _ s: Float					) -> ArraySlice< Float	>	{																																																																							return p + -s }
func	-			( _ p: ArraySlice< Double	>	, _ s: Double					) -> ArraySlice< Double	>	{																																																																							return p + -s }
func	*			( _ p: ArraySlice< Float	>	, _ s: Float					) -> ArraySlice< Float	>	{ var v = ArraySlice< Float	>( repeating: 0, count: p.count ); var w = s;	vDSP_vsmul		( p.withUnsafeBufferPointer { $0.baseAddress! }, 1, &w, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p.count ) );														return v }
func	*			( _ p: ArraySlice< Double	>	, _ s: Double					) -> ArraySlice< Double	>	{ var v = ArraySlice< Double>( repeating: 0, count: p.count ); var w = s;	vDSP_vsmulD		( p.withUnsafeBufferPointer { $0.baseAddress! }, 1, &w, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p.count ) );														return v }
func	/			( _ p: ArraySlice< Float	>	, _ s: Float					) -> ArraySlice< Float	>	{ var v = ArraySlice< Float	>( repeating: 0, count: p.count ); var w = s;	vDSP_vsdiv		( p.withUnsafeBufferPointer { $0.baseAddress! }, 1, &w, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p.count ) );														return v }
func	/			( _ p: ArraySlice< Double	>	, _ s: Double					) -> ArraySlice< Double	>	{ var v = ArraySlice< Double>( repeating: 0, count: p.count ); var w = s;	vDSP_vsdivD		( p.withUnsafeBufferPointer { $0.baseAddress! }, 1, &w, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p.count ) );														return v }

func	+			( _ s: Float					, _ p: ArraySlice< Float	>	) -> ArraySlice< Float	>	{																																																																							return p + s }
func	+			( _ s: Double					, _ p: ArraySlice< Double	>	) -> ArraySlice< Double	>	{																																																																							return p + s }
func	-			( _ s: Float					, _ p: ArraySlice< Float	>	) -> ArraySlice< Float	>	{																																																																							return -( p + -s ) }
func	-			( _ s: Double					, _ p: ArraySlice< Double	>	) -> ArraySlice< Double	>	{																																																																							return -( p + -s ) }
func	*			( _ s: Float					, _ p: ArraySlice< Float	>	) -> ArraySlice< Float	>	{																																																																							return p * s }
func	*			( _ s: Double					, _ p: ArraySlice< Double	>	) -> ArraySlice< Double	>	{																																																																							return p * s }
func	/			( _ s: Float					, _ p: ArraySlice< Float	>	) -> ArraySlice< Float	>	{ var v = ArraySlice< Float	>( repeating: 0, count: p.count ); var w = s;	vDSP_svdiv		( &w, p.withUnsafeBufferPointer { $0.baseAddress! }, 1, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p.count ) );														return v }
func	/			( _ s: Double					, _ p: ArraySlice< Double	>	) -> ArraySlice< Double	>	{ var v = ArraySlice< Double>( repeating: 0, count: p.count ); var w = s;	vDSP_svdivD		( &w, p.withUnsafeBufferPointer { $0.baseAddress! }, 1, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p.count ) );														return v }

//	----------------------------------------------------------------

func	Sum			( _ p: Vector< Float	>										) -> Float					{ var v: Float	= 0;														vDSP_sve		( p.m.withUnsafeBufferPointer { $0.baseAddress! }, p.s, &v, vDSP_Length( p.n ) );																												return v }
func	Sum			( _ p: Vector< Double	>										) -> Double					{ var v: Double	= 0;														vDSP_sveD		( p.m.withUnsafeBufferPointer { $0.baseAddress! }, p.s, &v, vDSP_Length( p.n ) );																												return v }
func	Mean		( _ p: Vector< Float	>										) -> Float					{ var v: Float	= 0;														vDSP_meanv		( p.m.withUnsafeBufferPointer { $0.baseAddress! }, p.s, &v, vDSP_Length( p.n ) );																												return v }
func	Mean		( _ p: Vector< Double	>										) -> Double					{ var v: Double	= 0;														vDSP_meanvD		( p.m.withUnsafeBufferPointer { $0.baseAddress! }, p.s, &v, vDSP_Length( p.n ) );																												return v }
func	L2NormQ		( _ p: Vector< Float	>										) -> Float					{ var v: Float	= 0;														vDSP_svesq		( p.m.withUnsafeBufferPointer { $0.baseAddress! }, p.s, &v, vDSP_Length( p.n ) );																												return v }
func	L2NormQ		( _ p: Vector< Double	>										) -> Double					{ var v: Double	= 0;														vDSP_svesqD		( p.m.withUnsafeBufferPointer { $0.baseAddress! }, p.s, &v, vDSP_Length( p.n ) );																												return v }
func	L2Norm		( _ p: Vector< Float	>										) -> Float					{																																																																							return sqrt( L2NormQ( p ) ) }
func	L2Norm		( _ p: Vector< Double	>										) -> Double					{																																																																							return sqrt( L2NormQ( p ) ) }
func	UnitVector	( _ p: Vector< Float	>										) -> Vector< Float	>		{																																																																							return p / L2Norm( p ) }
func	UnitVector	( _ p: Vector< Double	>										) -> Vector< Double	>		{																																																																							return p / L2Norm( p ) }
func	Abs			( _ p: Vector< Float	>										) -> Vector< Float	>		{ var v = ArraySlice< Float	>( repeating: 0, count: p.n );					vDSP_vabs		( p.m.withUnsafeBufferPointer { $0.baseAddress! }, p.s, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p.n ) );															return Vector( v ) }
func	Abs			( _ p: Vector< Double	>										) -> Vector< Double	>		{ var v = ArraySlice< Double>( repeating: 0, count: p.n );					vDSP_vabsD		( p.m.withUnsafeBufferPointer { $0.baseAddress! }, p.s, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p.n ) );															return Vector( v ) }
prefix func -		( _ p: Vector< Float	>										) -> Vector< Float	>		{ var v = ArraySlice< Float	>( repeating: 0, count: p.n );					vDSP_vneg		( p.m.withUnsafeBufferPointer { $0.baseAddress! }, p.s, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p.n ) );															return Vector( v ) }
prefix func -		( _ p: Vector< Double	>										) -> Vector< Double	>		{ var v = ArraySlice< Double>( repeating: 0, count: p.n );					vDSP_vnegD		( p.m.withUnsafeBufferPointer { $0.baseAddress! }, p.s, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p.n ) );															return Vector( v ) }
func	DistanceQ	( _ l: Vector< Float	>		, _ r: Vector< Float	>		) -> Float					{ var v: Float	= 0;														vDSP_distancesq	( l.m.withUnsafeBufferPointer { $0.baseAddress! }, l.s, r.m.withUnsafeBufferPointer { $0.baseAddress! }, r.s, &v, vDSP_Length( l.n ) );															return v }
func	DistanceQ	( _ l: Vector< Double	>		, _ r: Vector< Double	>		) -> Double					{ var v: Double	= 0;														vDSP_distancesqD( l.m.withUnsafeBufferPointer { $0.baseAddress! }, l.s, r.m.withUnsafeBufferPointer { $0.baseAddress! }, r.s, &v, vDSP_Length( l.n ) );															return v }
func	Dot			( _ l: Vector< Float	>		, _ r: Vector< Float	>		) -> Float					{ var v: Float	= 0;														vDSP_dotpr		( r.m.withUnsafeBufferPointer { $0.baseAddress! }, r.s, l.m.withUnsafeBufferPointer { $0.baseAddress! }, l.s, &v, vDSP_Length( l.n ) );															return v }
func	Dot			( _ l: Vector< Double	>		, _ r: Vector< Double	>		) -> Double					{ var v: Double	= 0;														vDSP_dotprD		( r.m.withUnsafeBufferPointer { $0.baseAddress! }, r.s, l.m.withUnsafeBufferPointer { $0.baseAddress! }, l.s, &v, vDSP_Length( l.n ) );															return v }
func	+			( _ l: Vector< Float	>		, _ r: Vector< Float	>		) -> Vector< Float	>		{ var v = ArraySlice< Float	>( repeating: 0, count: l.n );					vDSP_vadd		( l.m.withUnsafeBufferPointer { $0.baseAddress! }, l.s, r.m.withUnsafeBufferPointer { $0.baseAddress! }, r.s, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( l.n ) );	return Vector( v ) }
func	+			( _ l: Vector< Double	>		, _ r: Vector< Double	>		) -> Vector< Double	>		{ var v = ArraySlice< Double>( repeating: 0, count: l.n );					vDSP_vaddD		( l.m.withUnsafeBufferPointer { $0.baseAddress! }, l.s, r.m.withUnsafeBufferPointer { $0.baseAddress! }, r.s, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( l.n ) );	return Vector( v ) }
func	-			( _ l: Vector< Float	>		, _ r: Vector< Float	>		) -> Vector< Float	>		{ var v = ArraySlice< Float	>( repeating: 0, count: l.n );					vDSP_vsub		( r.m.withUnsafeBufferPointer { $0.baseAddress! }, r.s, l.m.withUnsafeBufferPointer { $0.baseAddress! }, l.s, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( l.n ) );	return Vector( v ) }
func	-			( _ l: Vector< Double	>		, _ r: Vector< Double	>		) -> Vector< Double	>		{ var v = ArraySlice< Double>( repeating: 0, count: l.n );					vDSP_vsubD		( r.m.withUnsafeBufferPointer { $0.baseAddress! }, r.s, l.m.withUnsafeBufferPointer { $0.baseAddress! }, l.s, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( l.n ) );	return Vector( v ) }
func	*			( _ l: Vector< Float	>		, _ r: Vector< Float	>		) -> Vector< Float	>		{ var v = ArraySlice< Float	>( repeating: 0, count: l.n );					vDSP_vmul		( l.m.withUnsafeBufferPointer { $0.baseAddress! }, l.s, r.m.withUnsafeBufferPointer { $0.baseAddress! }, r.s, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( l.n ) );	return Vector( v ) }
func	*			( _ l: Vector< Double	>		, _ r: Vector< Double	>		) -> Vector< Double	>		{ var v = ArraySlice< Double>( repeating: 0, count: l.n );					vDSP_vmulD		( l.m.withUnsafeBufferPointer { $0.baseAddress! }, l.s, r.m.withUnsafeBufferPointer { $0.baseAddress! }, r.s, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( l.n ) );	return Vector( v ) }
func	/			( _ l: Vector< Float	>		, _ r: Vector< Float	>		) -> Vector< Float	>		{ var v = ArraySlice< Float	>( repeating: 0, count: l.n );					vDSP_vdiv		( r.m.withUnsafeBufferPointer { $0.baseAddress! }, r.s, l.m.withUnsafeBufferPointer { $0.baseAddress! }, l.s, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( l.n ) );	return Vector( v ) }
func	/			( _ l: Vector< Double	>		, _ r: Vector< Double	>		) -> Vector< Double	>		{ var v = ArraySlice< Double>( repeating: 0, count: l.n );					vDSP_vdivD		( r.m.withUnsafeBufferPointer { $0.baseAddress! }, r.s, l.m.withUnsafeBufferPointer { $0.baseAddress! }, l.s, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( l.n ) );	return Vector( v ) }

func	+			( _ p: Vector< Float	>		, _ s: Float					) -> Vector< Float	>		{ var v = ArraySlice< Float	>( repeating: 0, count: p.n ); var w = s;		vDSP_vsadd		( p.m.withUnsafeBufferPointer { $0.baseAddress! }, p.s, &w, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p.n ) );														return Vector( v ) }
func	+			( _ p: Vector< Double	>		, _ s: Double					) -> Vector< Double	>		{ var v = ArraySlice< Double>( repeating: 0, count: p.n ); var w = s;		vDSP_vsaddD		( p.m.withUnsafeBufferPointer { $0.baseAddress! }, p.s, &w, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p.n ) );														return Vector( v ) }
func	-			( _ p: Vector< Float	>		, _ s: Float					) -> Vector< Float	>		{																																																																							return p + -s }
func	-			( _ p: Vector< Double	>		, _ s: Double					) -> Vector< Double	>		{																																																																							return p + -s }
func	*			( _ p: Vector< Float	>		, _ s: Float					) -> Vector< Float	>		{ var v = ArraySlice< Float	>( repeating: 0, count: p.n ); var w = s;		vDSP_vsmul		( p.m.withUnsafeBufferPointer { $0.baseAddress! }, p.s, &w, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p.n ) );														return Vector( v ) }
func	*			( _ p: Vector< Double	>		, _ s: Double					) -> Vector< Double	>		{ var v = ArraySlice< Double>( repeating: 0, count: p.n ); var w = s;		vDSP_vsmulD		( p.m.withUnsafeBufferPointer { $0.baseAddress! }, p.s, &w, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p.n ) );														return Vector( v ) }
func	/			( _ p: Vector< Float	>		, _ s: Float					) -> Vector< Float	>		{ var v = ArraySlice< Float	>( repeating: 0, count: p.n ); var w = s;		vDSP_vsdiv		( p.m.withUnsafeBufferPointer { $0.baseAddress! }, p.s, &w, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p.n ) );														return Vector( v ) }
func	/			( _ p: Vector< Double	>		, _ s: Double					) -> Vector< Double	>		{ var v = ArraySlice< Double>( repeating: 0, count: p.n ); var w = s;		vDSP_vsdivD		( p.m.withUnsafeBufferPointer { $0.baseAddress! }, p.s, &w, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p.n ) );														return Vector( v ) }

func	+			( _ s: Float					, _ p: Vector< Float	>		) -> Vector< Float	>		{																																																																							return p + s }
func	+			( _ s: Double					, _ p: Vector< Double	>		) -> Vector< Double	>		{																																																																							return p + s }
func	-			( _ s: Float					, _ p: Vector< Float	>		) -> Vector< Float	>		{																																																																							return -( p + -s ) }
func	-			( _ s: Double					, _ p: Vector< Double	>		) -> Vector< Double	>		{																																																																							return -( p + -s ) }
func	*			( _ s: Float					, _ p: Vector< Float	>		) -> Vector< Float	>		{																																																																							return p * s }
func	*			( _ s: Double					, _ p: Vector< Double	>		) -> Vector< Double	>		{																																																																							return p * s }
func	/			( _ s: Float					, _ p: Vector< Float	>		) -> Vector< Float	>		{ var v = ArraySlice< Float	>( repeating: 0, count: p.n ); var w = s;		vDSP_svdiv		( &w, p.m.withUnsafeBufferPointer { $0.baseAddress! }, p.s, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p.n ) );														return Vector( v ) }
func	/			( _ s: Double					, _ p: Vector< Double	>		) -> Vector< Double	>		{ var v = ArraySlice< Double>( repeating: 0, count: p.n ); var w = s;		vDSP_svdivD		( &w, p.m.withUnsafeBufferPointer { $0.baseAddress! }, p.s, v.withUnsafeMutableBufferPointer { $0.baseAddress! }, 1, vDSP_Length( p.n ) );														return Vector( v ) }
