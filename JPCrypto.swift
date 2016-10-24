//	To use those functions below
//	You need to import CommonCrypto.h in your bridging header.
//		#import <CommonCrypto/CommonCrypto.h>

import	Foundation

func
SHA1( _ p: Data ) -> Data {
	var v = [ UInt8 ]( repeating: 0, count: Int( CC_SHA1_DIGEST_LENGTH ) )
	p.withUnsafeBytes { ( q: UnsafePointer<UInt8> ) -> Void in
		CC_SHA1( q, CC_LONG( p.count ), &v )
	}
	return Data( bytes: v )
}


func
DataCryptedByAES( _ operation: CCOperation, _ p: Data, _ key: Data, _ options: CCOptions = CCOptions( kCCOptionPKCS7Padding ), _ iv: Data? = nil ) -> ( CCCryptorStatus, Data ) {

	assert ( key.count == kCCKeySizeAES128 || key.count == kCCKeySizeAES192 || key.count == kCCKeySizeAES256 )

	if iv != nil { assert( iv!.count == kCCBlockSizeAES128 ) }
	
	var	wBytes		: UnsafePointer<UInt8>
	p.withUnsafeBytes { ( q: UnsafePointer<UInt8> ) -> Void in wBytes = q }
	var	wKeyBytes	: UnsafePointer<UInt8>
	key.withUnsafeBytes { ( q: UnsafePointer<UInt8> ) -> Void in wKeyBytes = q }
	var	wIVBytes	: UnsafePointer<UInt8>
	if let wIV = iv { wIV.withUnsafeBytes { ( q: UnsafePointer<UInt8> ) -> Void in wIVBytes = q } }
	
	var	wLength = size_t( ( ( p.count + kCCBlockSizeAES128 - 1 ) / kCCBlockSizeAES128 ) * kCCBlockSizeAES128 )
	let v = [ UInt8 ]( repeating: 0, count: Int( wLength ) )
	let	s = CCCrypt(
		operation
	,	CCAlgorithm( kCCAlgorithmAES )
	,	options
	,	wKeyBytes
	,	key.count
	,	wIVBytes
	,	wBytes
	,	size_t( p.count )
	,	UnsafeMutableRawPointer( mutating: v )
	,	wLength
	,	&wLength
	)
	return ( s, Data( bytes: v ) )
}

func
DataEncryptedByAES( _ p: Data, _ key: Data, _ options: CCOptions = CCOptions( kCCOptionPKCS7Padding ), _ iv: Data? = nil ) -> ( CCCryptorStatus, Data ) {
	return DataCryptedByAES( CCOperation( kCCEncrypt ), p, key, options, iv )
}

func
DataDecryptedByAES( _ p: Data, _ key: Data, _ options: CCOptions = CCOptions( kCCOptionPKCS7Padding ), _ iv: Data? = nil ) -> ( CCCryptorStatus, Data ) {
	return DataCryptedByAES( CCOperation( kCCDecrypt ), p, key, options, iv )
}

func
DataCryptedByBlowfish( _ operation: CCOperation, _ p: Data, _ key: Data, _ options: CCOptions = CCOptions( kCCOptionPKCS7Padding ), _ iv: Data? = nil ) -> ( CCCryptorStatus, Data ) {

	if iv != nil { assert( iv!.count == kCCBlockSizeBlowfish ) }

	var	wBytes		: UnsafePointer<UInt8>
	p.withUnsafeBytes { ( q: UnsafePointer<UInt8> ) -> Void in wBytes = q }
	var	wKeyBytes	: UnsafePointer<UInt8>
	key.withUnsafeBytes { ( q: UnsafePointer<UInt8> ) -> Void in wKeyBytes = q }
	var	wIVBytes	: UnsafePointer<UInt8>
	if let wIV = iv { wIV.withUnsafeBytes { ( q: UnsafePointer<UInt8> ) -> Void in wIVBytes = q } }
	
	var	wLength = size_t( ( ( p.count + kCCBlockSizeBlowfish - 1 ) / kCCBlockSizeBlowfish ) * kCCBlockSizeBlowfish )
	let v = [ UInt8 ]( repeating: 0, count: Int( wLength ) )
	let	s = CCCrypt(
		operation
	,	CCAlgorithm( kCCAlgorithmBlowfish )
	,	options
	,	wKeyBytes
	,	key.count
	,	wIVBytes
	,	wBytes
	,	size_t( p.count )
	,	UnsafeMutableRawPointer( mutating: v )
	,	wLength
	,	&wLength
	)
	return ( s, Data( bytes: v ) )
}

func
DataEncryptedByBlowfish( _ p: Data, _ key: Data, _ options: CCOptions = CCOptions( kCCOptionPKCS7Padding ), _ iv: Data? = nil ) -> ( CCCryptorStatus, Data ) {
	return DataCryptedByBlowfish( CCOperation( kCCEncrypt ), p, key, options, iv )
}

func
DataDecryptedByBlowfish( _ p: Data, _ key: Data, _ options: CCOptions = CCOptions( kCCOptionPKCS7Padding ), _ iv: Data? = nil ) -> ( CCCryptorStatus, Data ) {
	return DataCryptedByBlowfish( CCOperation( kCCDecrypt ), p, key, options, iv )
}

