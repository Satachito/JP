//	To find out the latest version, refer to the following address: https://github.com/firebase/firebase-js-sdk/tags

////////////////////////////////////////////////////////////////	v	app
import {
	initializeApp
} from 'https://www.gstatic.com/firebasejs/10.10.0/firebase-app.js'

////////////////////////////////////////////////////////////////	v	Analytics
import {
	getAnalytics
} from 'https://www.gstatic.com/firebasejs/10.10.0/firebase-analytics.js'

////////////////////////////////////////////////////////////////	v	Auth
import { 
	connectAuthEmulator
,	getAuth
,	getIdToken
,	onAuthStateChanged
,	signInWithEmailAndPassword
,	signInWithPopup
,	signInWithRedirect
,	sendEmailVerification
,	createUserWithEmailAndPassword
,	sendPasswordResetEmail
,	GoogleAuthProvider
,	FacebookAuthProvider
,	GithubAuthProvider
,	signOut
,	deleteUser
,	getRedirectResult
} from 'https://www.gstatic.com/firebasejs/10.10.0/firebase-auth.js'


////////////////////////////////////////////////////////////////	v	Firestore
import {
	connectFirestoreEmulator
,	getFirestore
} from 'https://www.gstatic.com/firebasejs/10.10.0/firebase-firestore.js'

////////////////////////////////////////////////////////////////	v	storage
import {
	connectStorageEmulator
,	getStorage
} from 'https://www.gstatic.com/firebasejs/10.10.0/firebase-storage.js'

////////////////////////////////////////////////////////////////	v

let	app
let	analystics
let	auth
let	db
let	storage
let	ThenCB = ( tag, email ) => {}
let	CatchCB = ( tag, er ) => console.log( tag, er )

export const
User							= ()	=> auth.currentUser

export const
Email							= ()	=> User() && User().email

const
TC = ( _, tag ) => _().then( _	=> ThenCB( tag, Email()	) ).catch( er => CatchCB( tag, er ) )

export const
InitializeApp = ( _, thenCB, catchCB ) => (
	app			= initializeApp( _ )
,	analystics	= getAnalytics	( app )
,	auth		= getAuth		( app )
,	db			= getFirestore	( app )
,	storage		= getStorage( app )
,	location.hostname === 'localhost' && (
		connectAuthEmulator			( auth		, 'http://localhost:9099'	)
	,	connectFirestoreEmulator	( db		, 'localhost', 8080			)
	,	connectStorageEmulator		( storage	, 'localhost', 9199			)
	)
,	ThenCB	= thenCB
,	CatchCB	= catchCB
,	TC( () => getRedirectResult( auth ), 'getRedirectResult' )
)

//	mail		-> Google	: mail/google
//	Facebook	-> Google	: facebook.com/google.com
//	GitHub		-> Google	: github.com/google.com
//	Any			-> mail		: email-already-in-use
//	Any			-> Facebook	: account-exists-with-different-credential
//	Any			-> GitHub	: account-exists-with-different-credential

export const
OnAuthStateChanged				= _ => onAuthStateChanged( auth, _ )

export const
SignInWithGoogleRedirect		= () => signInWithRedirect( auth, new GoogleAuthProvider()		)	//	redirect されるのでここには来ない
export const
SignInWithFacebookRedirect		= () => signInWithRedirect( auth, new FacebookAuthProvider()	)	//	redirect されるのでここには来ない
export const
SignInWithGitHubRedirect		= () => signInWithRedirect( auth, new GitHubAuthProvider()		)	//	redirect されるのでここには来ない

export const
SignInWithGooglePopup			= () => TC( () => signInWithPopup( auth, new GoogleAuthProvider()		), 'signInWithPopup(Google)'		)
export const
SignInWithFacebookPopup			= () => TC( () => signInWithPopup( auth, new FacebookAuthProvider()		), 'signInWithPopup(Facebook)'	)
export const
SignInWithGitHubPopup			= () => TC( () => signInWithPopup( auth, new GitHubAuthProvider()		), 'signInWithPopup(GitHub)'		)

export const
GetIdToken						= () => TC( () => getIdToken( User(), true								), 'getIdToken'						)


export const
CreateUserWithEmailAndPassword	= ( email, password )	=> TC( () => createUserWithEmailAndPassword	( auth, email, password	), 'createUserWithEmailAndPassword'	)
export const
SignInWithEmailAndPassword		= ( email, password )	=> TC( () => signInWithEmailAndPassword		( auth, email, password ), 'signInWithEmailAndPassword'		)

const
TX = ( _, tag, email	) => _().then( ()	=> ThenCB( tag, email	) ).catch( er => CatchCB( tag, er ) )

export const
SendPasswordResetEmail			= email					=> TX( () => sendPasswordResetEmail	( auth, email					), 'sendPasswordResetEmail'			, email )
export const
SendEmailVerification			= ( email = Email() )	=> TX( () => sendEmailVerification	( User()						), 'sendEmailVerification'			, email )
export const
DeleteUser						= ( email = Email() )	=> TX( () => deleteUser				( User()						), 'deleteUser'						, email )
export const
SignOut							= ( email = Email() )	=> TX( () => signOut				( auth							), 'signOut'						, email )
