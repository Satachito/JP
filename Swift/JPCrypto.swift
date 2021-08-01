//	Written by Satoru Ogura, Tokyo.
//
import	Foundation

func
SHA1( _ p: [ UInt8 ] ) -> [ UInt8 ] {
	var v = [ UInt8 ]( repeating: 0, count: Int( CC_SHA1_DIGEST_LENGTH ) )
	CC_SHA1( p, CC_LONG( p.count ), &v )
	return v
}

func
Crypt(
	_ p			: [ UInt8 ]
,	_ key		: [ UInt8 ]
,	_ operation	: CCOperation
,	_ alg		: CCAlgorithm
,	_ blockSize	: Int
,	_ options	: CCOptions = CCOptions( kCCOptionPKCS7Padding )
,	_ iv		: [ UInt8 ]? = nil
) -> [ UInt8 ] {

	var	wLength = size_t( ( ( p.count + blockSize - 1 ) / blockSize ) * blockSize )
	let v = [ UInt8 ]( repeating: 0, count: Int( wLength ) )
	guard CCCrypt(
		operation
	,	CCAlgorithm( kCCAlgorithmAES )
	,	options
	,	key
	,	key.count
	,	iv
	,	p
	,	size_t( p.count )
	,	UnsafeMutableRawPointer( mutating: v )
	,	wLength
	,	&wLength
	) == kCCSuccess else { fatalError() }
	return v
}

func
AESCrypt(
	_ p			: [ UInt8 ]
,	_ key		: [ UInt8 ]
,	_ operation	: CCOperation
,	_ options	: CCOptions = CCOptions( kCCOptionPKCS7Padding )
,	_ iv		: [ UInt8 ]? = nil
) -> [ UInt8 ] {
	guard key.count == kCCKeySizeAES128 || key.count == kCCKeySizeAES192 || key.count == kCCKeySizeAES256 else { fatalError() }
	return Crypt(
		p
	,	key
	,	operation
	,	CCAlgorithm( kCCAlgorithmAES )
	,	kCCBlockSizeAES128
	,	options
	,	iv
	)
}

func
AESEncrypt( _ p: [ UInt8 ], _ key: [ UInt8 ], _ options: CCOptions = CCOptions( kCCOptionPKCS7Padding ), _ iv: [ UInt8 ]? = nil ) -> [ UInt8 ] {
	return AESCrypt( p, key, CCOperation( kCCEncrypt ), options, iv )
}

func
AESDecrypt( _ p: [ UInt8 ], _ key: [ UInt8 ], _ options: CCOptions = CCOptions( kCCOptionPKCS7Padding ), _ iv: [ UInt8 ]? = nil ) -> [ UInt8 ] {
	return AESCrypt( p, key, CCOperation( kCCDecrypt ), options, iv )
}

func
BlowfishCrypt(
	_ p			: [ UInt8 ]
,	_ key		: [ UInt8 ]
,	_ operation	: CCOperation
,	_ options	: CCOptions = CCOptions( kCCOptionPKCS7Padding )
,	_ iv		: [ UInt8 ]? = nil
) -> [ UInt8 ] {
	return Crypt(
		p
	,	key
	,	operation
	,	CCAlgorithm( kCCAlgorithmBlowfish )
	,	kCCBlockSizeBlowfish
	,	options
	,	iv
	)
}

func
BlowfishEncrypt( _ p: [ UInt8 ], _ key: [ UInt8 ], _ options: CCOptions = CCOptions( kCCOptionPKCS7Padding ), _ iv: [ UInt8 ]? = nil ) -> [ UInt8 ] {
	return BlowfishCrypt( p, key, CCOperation( kCCEncrypt ), options, iv )
}

func
BlowfishDecrypt( _ p: [ UInt8 ], _ key: [ UInt8 ], _ options: CCOptions = CCOptions( kCCOptionPKCS7Padding ), _ iv: [ UInt8 ]? = nil ) -> [ UInt8 ] {
	return BlowfishCrypt( p, key, CCOperation( kCCDecrypt ), options, iv )
}
