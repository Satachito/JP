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
let	ThenCB
let	CatchCB

const
TC = ( _, tag			) => _().then( _	=> ThenCB( tag, Email()	) ).catch( er => CatchCB( tag, er ) )
const
TX = ( _, tag, email	) => _().then( ()	=> ThenCB( tag, email	) ).catch( er => CatchCB( tag, er ) )

export let app
export let analystics
export let auth
export let db
export let storage

export const
InitializeApp = ( _, thenCB = ( tag, email ) => {}, catchCB = console.error ) => (
	app			= initializeApp	( _ )
,	analystics	= getAnalytics	( app )
,	auth		= getAuth		( app )
,	db			= getFirestore	( app )
,	storage		= getStorage	( app )
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
User							= () => auth.currentUser
export const
Email							= () => User() && User().email

export const
OnAuthStateChanged				= _ => onAuthStateChanged( auth, _ )
export const
GetIdToken						= () => getIdToken( User(), true )

export const
SignInWithGoogleRedirect		= () => signInWithRedirect( auth, new GoogleAuthProvider()		)	//	redirect されるのでここには来ない
export const
SignInWithFacebookRedirect		= () => signInWithRedirect( auth, new FacebookAuthProvider()	)	//	redirect されるのでここには来ない
export const
SignInWithGitHubRedirect		= () => signInWithRedirect( auth, new GitHubAuthProvider()		)	//	redirect されるのでここには来ない

export const
SignInWithGooglePopup			= () => TC( () => signInWithPopup( auth, new GoogleAuthProvider()	), 'signInWithPopup(Google)'	)
export const
SignInWithFacebookPopup			= () => TC( () => signInWithPopup( auth, new FacebookAuthProvider()	), 'signInWithPopup(Facebook)'	)
export const
SignInWithGitHubPopup			= () => TC( () => signInWithPopup( auth, new GitHubAuthProvider()	), 'signInWithPopup(GitHub)'	)

export const
CreateUserWithEmailAndPassword	= ( email, password )	=> TC( () => createUserWithEmailAndPassword	( auth, email, password	), 'createUserWithEmailAndPassword'	)
export const
SignInWithEmailAndPassword		= ( email, password )	=> TC( () => signInWithEmailAndPassword		( auth, email, password ), 'signInWithEmailAndPassword'		)

export const
SendEmailVerification			= ( email = Email() )	=> TX( () => sendEmailVerification	( User()		), 'sendEmailVerification'	, email )
export const
DeleteUser						= ( email = Email() )	=> TX( () => deleteUser				( User()		), 'deleteUser'				, email )
export const
SignOut							= ( email = Email() )	=> TX( () => signOut				( auth			), 'signOut'				, email )

export const
SendPasswordResetEmail			= email					=> TX( () => sendPasswordResetEmail	( auth, email	), 'sendPasswordResetEmail'	, email )
