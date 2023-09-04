#pragma once

template < typename I >	inline string
HexString( vector< I > const& _ ) {
	static_assert( is_unsigned< I >::value, "eh?" );

	string $;
	for ( auto& _: _ ) $ = EncodeHex( _ ) + $;
	return $;
}

template < typename I > inline auto
ShiftLeftInplace( vector< I >& $, UI8 _ ) {
	static_assert( is_unsigned< I >::value, "eh?");

	constexpr UI8 N_BITS_PER_I = sizeof( I ) * 8;

	auto nBits = _ % N_BITS_PER_I;
	if ( nBits ) {
		auto _$ = $.size();
		while ( --_$ ) $[ _$ ] = ( $[ _$ ] << nBits ) | ( $[ _$ - 1 ] >> ( N_BITS_PER_I - nBits ) );
		$[ _$ ] <<= nBits;
	}
	auto nIs = _ / N_BITS_PER_I;
	if ( nIs ) {
		auto _$ = $.size();
		while ( _$ - nIs ) {
			_$--;
			$[ _$ ] = $[ _$ - nIs ];
		}
		while ( _$-- ) $[ _$ ] = 0;
	}
}

template < typename I > inline auto
ShiftRightInplace( vector< I >& $, UI8 _ ) {
	static_assert( is_unsigned< I >::value, "eh?");

	constexpr UI8 N_BITS_PER_I = sizeof( I ) * 8;

	auto nBits = _ % N_BITS_PER_I;
	if ( nBits ) {
		auto _$ = 0;
		while ( _$ < $.size() - 1 ) {
			$[ _$ ] = ( $[ _$ ] >> nBits ) | ( $[ _$ + 1 ] << ( N_BITS_PER_I - nBits ) );
			_$++;
		}
		$[ _$ ] >>= nBits;
	}
	auto nIs = _ / N_BITS_PER_I;
	if ( nIs ) {
		auto _$ = 0;
		while ( _$ < $.size() - nIs ) {
			$[ _$ ] = $[ _$ + nIs ];
			_$++;
		}
		while ( _$ < $.size() ) $[ _$++ ] = 0;
	}
}

template < typename I > inline auto
AddOneInplace( vector< I >& $, UI8 _, I a ) {
	static_assert( is_unsigned< I >::value, "eh?" );

	$[ _ ] += a;
	if ( $[ _++ ] < a ) {
		while ( _ < $.size() ) {
			if ( ++$[ _ ] ) break;
			_++;
		}
		if ( _ == $.size() ) $.push_back( 1 );
	}
};

template < typename I > inline vector< I >
Not( vector< I > $ ) {
	static_assert( is_unsigned< I >::value, "eh?");

	auto _ = $.size();
	$.resize( _ );
	while ( _-- ) $[ _ ] = ~$[ _ ];
	while ( !$.empty() && $.back() == 0 ) $.pop_back();
	return $;
}

template < typename I > inline vector< I >
And( vector< I > const& $, vector< I > const& _ ) {
	static_assert( is_unsigned< I >::value, "eh?");

	auto _And = []( vector< I > $, vector< I > const& _ ) {
		auto _$ = _.size();
		$.resize( _$ );
		while ( _$-- ) $[ _$ ] &= _[ _$ ];
		while ( !$.empty() && $.back() == 0 ) $.pop_back();
		return $;
	};
	return GEL( $.size(), _.size() ) < 0 ? _And( _, $ ) : _And( $, _ );
}

template < typename I > inline vector< I >
Or( vector< I > $, vector< I > const& _ ) {
	static_assert( is_unsigned< I >::value, "eh?");

	auto _Or = []( vector< I > $, vector< I > const& _ ) {
		auto _$ = _.size();
		while ( _$-- ) $[ _$ ] |= _[ _$ ];
		return $;
	};
	return GEL( $.size(), _.size() ) < 0 ? _Or( _, $ ) : _Or( $, _ );
}

template < typename I > inline vector< I >
Xor( vector< I > $, vector< I > const& _ ) {
	static_assert( is_unsigned< I >::value, "eh?");

	auto _Xor = []( vector< I > $, vector< I > const& _ ) {
		auto _$ = _.size();
		while ( _$-- ) $[ _$ ] ^= _[ _$ ];
		return $;
	};
	return GEL( $.size(), _.size() ) < 0 ? _Xor( _, $ ) : _Xor( $, _ );
}

template < typename I > inline vector< I >
Add( vector< I > $, vector< I > const& _ ) {
	static_assert( is_unsigned< I >::value, "eh?");

	auto _Add = []( vector< I > $, vector< I > const& _ ) {
		bool carry = false;
		UI8 _$ = 0;
		while ( _$ < _.size() ) {
			auto A = _[ _$ ] + ( carry ? 1 : 0 );
			$[ _$ ] += A;
			carry = $[ _$++ ] < A;
		}
		while ( carry && _$ < $.size() ) carry = ++$[ _$++ ] == 0;
		if ( carry ) $.push_back( 1 );
		return $;
	};
	return GEL( $.size(), _.size() ) < 0 ? _Add( _, $ ) : _Add( $, _ );
}

template < typename I > inline auto	//	$ must be greater than _
Sub( vector< I > $, vector< I > const& _ ) {
	static_assert( is_unsigned< I >::value, "eh?" );

	bool borrow = false;
	UI8 _$ = 0;
	while ( _$ < _.size() ) {
		auto A = _[ _$ ] + ( borrow ? 1 : 0 );
		borrow = $[ _$ ] < A;
		$[ _$++ ] -= A;
	}
	while ( borrow && _$ < $.size() ) borrow = $[ _$++ ]-- == 0;
	assert( !borrow );
	while ( !$.empty() && $.back() == 0 ) $.pop_back();
	return $;
}

template < typename I > inline pair< I, I >
Mul( I p, I q ) {
	static_assert( is_unsigned< I >::value, "eh?" );

	constexpr auto	SHIFT	= sizeof( I ) * 4;
	constexpr I		MASK	= ( I( 1 ) << SHIFT ) - 1;

	I pL = p & MASK;
	I qL = q & MASK;
	I pH = p >> SHIFT;
	I qH = q >> SHIFT;

	I L_L = pL * qL;
	I L_H = pL * qH;
	I H_L = pH * qL;
	I H_H = pH * qH;

	I L = L_L + ( L_H << SHIFT ) + ( H_L << SHIFT );

	return {
		H_H + ( L_H >> SHIFT ) + ( H_L >> SHIFT ) + ( L < L_L ? 1 : 0 )
	,	L
	};
}

template < typename I > inline auto
Mul( vector< I > const& p, vector< I > const& q ) {
	static_assert( is_unsigned< I >::value, "eh?" );

	vector< I >	$( p.size() + q.size() );
	for ( auto _P = 0; _P < p.size(); _P++ ) {
		for ( auto _Q = 0; _Q < q.size(); _Q++ ) {
			auto A = Mul( p[ _P ], q[ _Q ] );
			AddOneInplace( $, _P + _Q		, A.second );
			AddOneInplace( $, _P + _Q + 1	, A.first );
		}
	}
	while ( !$.empty() && $.back() == 0 ) $.pop_back();
	return $;
}

template < typename I > inline auto
Mul( vector< I > const& p, I q ) {
	static_assert( is_unsigned< I >::value, "eh?" );

	vector< I >	$( p.size() + 1 );
	for ( auto _ = 0; _ < p.size(); _++ ) {
		auto A = Mul( p[ _ ], q );
		$[ _ ] += A.second;
		$[ _ + 1 ] = A.first;
		if ( $[ _ ] < A.second ) $[ _ + 1 ]++;
	}
	while ( !$.empty() && $.back() == 0 ) $.pop_back();
	return $;
}

template < typename I >	inline int
GEL( vector< I > const& p, vector< I> const& q ) {
	static_assert( is_unsigned< I >::value, "eh?" );

	auto _ = p.size();
	auto gel = GEL( _, q.size() );
	if ( gel ) return gel;

	while ( _-- ) {
		if ( p[ _ ] > q[ _ ] ) return 1;
		if ( p[ _ ] < q[ _ ] ) return -1;
	}
	return 0;
}
template < typename I > inline pair< vector< I >, vector< I > >
Div( vector< I > R, vector< I > const& D ) {
	static_assert( is_unsigned< I >::value, "eh?" );

	switch ( GEL( R, D ) ) {
	case -1	: return { vector< I >(), R };
	case 0	: return { vector< I >(), vector< I >() };
	}
	//	R > D garanteed

	constexpr UI8 N_BITS_PER_I = sizeof( I ) * 8;

	auto nBitsD = D.size() * N_BITS_PER_I - NumLeadingZeroBits( D.back() );
	auto nBitsR = R.size() * N_BITS_PER_I - NumLeadingZeroBits( R.back() );
	auto nShiftBits = nBitsR - nBitsD;

	//	R > D garanteed, so if nShiftBits == 0 quotient is 1.
	if ( nShiftBits == 0 ) return { { 1 }, Sub( R, D ) };

	vector< I > $( R.size() );	// Quotient
	vector< I > _( R.size() );	// shifted divisor, has size of R
	for ( UI8 _D = 0; _D < D.size(); _D++ ) _[ _D ] = D[ _D ];
	ShiftLeftInplace( _, nShiftBits );	// No needs to trim
	
	//	GENERAL CAUTION! GEL first looks in each size and compare them.
	
	switch ( GEL( R, _ ) ) {
	case 0:
		$[ nShiftBits / N_BITS_PER_I ] |= I( 1 ) << ( nShiftBits % N_BITS_PER_I );
		while ( !$.empty() && $.back() == 0 ) $.pop_back();
		return { $, vector< I >() };
	case -1:
		nShiftBits--;
		ShiftRightInplace( _, 1 );
	//	while ( !_.empty() && _.back() == 0 ) _.pop_back();
	//		Here, _.size() remain same as R, so next GEL( R, D ) works correctly.
		break;
	}
	while ( GEL( R, D ) != -1 ) {
		$[ nShiftBits / N_BITS_PER_I ] |= I( 1 ) << ( nShiftBits % N_BITS_PER_I );
		R = Sub( R, _ );
		if ( R.size() == 0 ) break;
		while ( nShiftBits ) {
			nShiftBits--;
			ShiftRightInplace( _, 1 );
			while ( !_.empty() && _.back() == 0 ) _.pop_back();	// NEEDS TRIM !
			if ( GEL( R, _ ) != -1 ) break;
		}
	}

	while ( !$.empty() && $.back() == 0 ) $.pop_back();
	return { $, R };
}

template < typename I >	inline string
NumString( I R, UI8 radix ) {
	static_assert( is_unsigned< I >::value, "eh?" );

	vector< I > divisors{ 1 };
	auto _ = (UI8)( log2( R ) / log2( radix ) );
	while ( _-- ) divisors.emplace_back( divisors.back() * radix );

    std::string $;
	while ( !divisors.empty() ) {
		auto divisor = divisors.back();
		$ += DigitCharacter( R / divisor );
		divisors.pop_back();
		R = R % divisor;
	}
	return $;
}

template < typename I >	inline string
NumString( vector< I > R, UI8 radix ) {
	static_assert( is_unsigned< I >::value, "eh?" );

	constexpr		UI8			N_BITS_PER_I	= sizeof( I ) * 8;

	vector< vector< I > >	divisors{ vector< I >{ 1 } };
	auto nBits = R.size() * N_BITS_PER_I - NumLeadingZeroBits( R.back() );
	auto _ = UI8( nBits / log2( radix ) );
	while ( --_ ) divisors.emplace_back( Mul( divisors.back(), (I)radix ) );
	
    std::string $;
	while ( !divisors.empty() ) {
		auto _ = Div( R, divisors.back() );
		divisors.pop_back();
		$ += _.first.empty() ? "0" : NumString( _.first[ 0 ], radix );
		R = _.second;
	}
	
	return $;
}

inline double
Double( vector< UI8 > const& _ ) {
	return accumulate( _.rbegin(), _.rend(), 0.0, []( auto $, auto _ ){ return ( $ * 0x100000000 ) * 0x100000000 + double( _ ); } );
}
inline double
Double( vector< UI4 > const& _ ) {
	return accumulate( _.rbegin(), _.rend(), 0.0, []( auto $, auto _ ){ return $ * 0x100000000 + double( _ ); } );
}
inline double
Double( vector< UI2 > const& _ ) {
	return accumulate( _.rbegin(), _.rend(), 0.0, []( auto $, auto _ ){ return $ * 0x10000 + double( _ ); } );
}
inline double
Double( vector< UI1 > const& _ ) {
	return accumulate( _.rbegin(), _.rend(), 0.0, []( auto $, auto _ ){ return $ * 0x100 + double( _ ); } );
}
