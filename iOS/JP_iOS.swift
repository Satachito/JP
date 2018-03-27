import UIKit

func
FrameX( _ p: UIView, _ x: CGFloat ) {
	var	w = p.frame
	w.origin.x = x
	p.frame = w
}

func
FrameY( _ p: UIView, _ y: CGFloat ) {
	var	w = p.frame
	w.origin.y = y
	p.frame = w
}


func
InputBox(
  _	vc			: UIViewController
, _	title		: String
, _	message		: String? = nil
, _	config		: @escaping ( UITextField ) -> () = { _ in }
, _	ed			: @escaping ( UITextField ) -> ()
) {
	let wAC = UIAlertController(
		title			: title
	,	message			: message
	,	preferredStyle	: .alert
	)
	var wTF : UITextField!
	wAC.addTextField { p in
		config( p )
		wTF = p
	}
	wAC.addAction( UIAlertAction( title: "Cancel", style: .cancel, handler: nil ) )
	wAC.addAction( UIAlertAction( title: "OK", style: .default ) { _ in ed( wTF ) } )
	wAC.view.setNeedsLayout()
	vc.present(
		wAC
	,	animated:true
	,	completion:nil
	)
}
	
func
Animate( _ duration: TimeInterval = 0.25, p: @escaping () -> () ) {
	UIView.animate( withDuration: duration, animations: p )
}

func
SpringAnimate(
_	duration	: TimeInterval					= 0.25
,	delay		: TimeInterval					= 0
,	damping		: CGFloat						= 0.5
,	velocity	: CGFloat						= 0.1
,	options		: UIViewAnimationOptions		= .curveEaseInOut
,	completion	: ( ( Bool ) -> () )?	= nil
,	animations	: @escaping () -> ()
) {
	UIView.animate(
		withDuration			: duration
	,	delay					: delay
	,	usingSpringWithDamping	: damping
	,	initialSpringVelocity	: velocity
	,	options					: options
	,	animations				: animations
	,	completion				: completion
	)
}

func
BlockAlert(
  _	vc		: UIViewController
, _	title	: String? = nil
, _	message	: String? = nil
) -> UIAlertController {
	let	v = UIAlertController(
		title			 : title
	,	message			 : message
	,	preferredStyle	 : .alert
	)

	vc.present(
		v
	,	animated	 : true
	,	completion	 : nil
	)

	return v
}

func
Alert(
  _	vc		: UIViewController
, _	title	: String? = nil
, _	message	: String? = nil
, _	handler	: ( ( UIAlertAction ) -> () )? = { _ in }
) {
	let wAC = UIAlertController(
		title			 : title
	,	message			 : message
	,	preferredStyle	 : .alert
	)
	wAC.addAction( UIAlertAction( title: "OK", style: .cancel, handler: handler ) )
	vc.present(
		wAC
	,	animated	 : true
	,	completion	 : nil
	)
}

func
ErrorAlert(
  _	vc		: UIViewController
, _	p		: Error
, _	handler	: ( ( UIAlertAction ) -> () )? = { _ in }
) {
	Alert( vc, "Error", p.localizedDescription, handler )
}

func
HTMLAlert(
  _	vc		: UIViewController
, _	r		: HTTPURLResponse
, _	d		: Data
,	handler	: ( ( UIAlertAction ) -> () )? = { _ in }
 ) {
	Alert( vc, r.description, nil, handler )
}

func
Confirmation(
  _	vc		: UIViewController
, _	title	: String! = nil
, _	message	: String! = nil
, _	handler	: ( ( UIAlertAction ) -> () )? = { _ in }
) {
	let wAC = UIAlertController(
		title			 : title
	,	message			 : message
	,	preferredStyle	 : .alert
	)
	wAC.addAction( UIAlertAction( title: "OK", style: .default, handler: handler ) )
	wAC.addAction( UIAlertAction( title: "Cancel", style: .cancel, handler: handler ) )
	vc.present(
		wAC
	,	animated	: true
	,	completion	: nil
	)
}

func
Image( _ path: String ) -> UIImage! {
	do {
		return UIImage( data: try Data( contentsOf: URL( string: path )! ) )
	} catch {
		return nil
	}
}

class
V	: UIView {
	var drawer	: ( CGRect ) -> () = { _ in }
	override func
	draw( _ p: CGRect ) {
		super.draw( p )
		drawer( p )
	}
}

func
BoundingRectWithWidth( _ p: NSAttributedString, width: CGFloat ) -> CGRect {
	return p.boundingRect(
		with	: CGSize( width: width, height: .greatestFiniteMagnitude )
	,	options	: .usesLineFragmentOrigin
	,	context	: nil
	)
}

func
AdjustHeight( _ p: UITextView ) {
	var wRect = BoundingRectWithWidth(
		p.attributedText
	,	width: p.bounds.size.width - p.textContainerInset.left - p.textContainerInset.right - p.textContainer.lineFragmentPadding * 2
	)
	wRect.size.width += p.textContainerInset.left + p.textContainerInset.right + p.textContainer.lineFragmentPadding * 2
	wRect.size.height += p.textContainerInset.top + p.textContainerInset.bottom
	p.bounds = wRect
}

func
HTML_iOS(
  _	uri		: String
, _	method	: String
, _	body	: Data? = nil
, _	er		: @escaping ( Error ) -> () = { e in }
, _	ex		: @escaping ( HTTPURLResponse, Data ) -> () = { r, d in }
,	ed		: @escaping ( Data ) -> () = { p in }
) {
	OnHTML( uri, method, body, er, ex, ed )
}

func
JSON_iOS(
  _	uri		: String
, _	method	: String
, _	json	: AnyObject? = nil
, _	er		: @escaping ( Error ) -> () = { e in }
, _	ex		: @escaping ( HTTPURLResponse, Data ) -> () = { r, d in }
,	ed		: @escaping ( Any ) -> ()
) {
	OnJSON( uri, method, json, er, ex, ed )
}

func
Image_iOS(
  _	uri	: String
, _	er	: @escaping ( Error ) -> () = { e in }
, _	ex	: @escaping ( HTTPURLResponse, Data ) -> () = { r, d in }
,	ed	: @escaping ( UIImage ) -> ()
) {
	OnHTML( uri, "GET", nil, er, ex ) { p in
		if let wImage = UIImage( data: p ) {
			ed( wImage )
		} else {
			assert( false )
		}
	}
}

class
ImageV	: UIImageView {
	var
	aiStyle	= UIActivityIndicatorViewStyle.whiteLarge
	let
	label	= UILabel()
	
	func
	ShowMessage( _ p: String ) {
		label.text = p
		label.numberOfLines = 0
		label.frame = bounds
		addSubview( label )
	}
	
	var
	uri	: String? {
		didSet {
			label.removeFromSuperview()
			let	wAIV = UIActivityIndicatorView( activityIndicatorStyle: aiStyle )
			addSubview( wAIV )
			wAIV.center = Center( bounds )
			wAIV.startAnimating()
			Image_iOS(
				uri!
			,	{	e in
					wAIV.removeFromSuperview()
					self.ShowMessage( "Error" )
				}
			,	{	r, d in
					wAIV.removeFromSuperview()
					self.ShowMessage( r.description )
				}
			) { p in
				wAIV.removeFromSuperview()
				self.image = p
			}
		}
	}
}

func
ARGBColor( _ p: Int ) -> UIColor {
    return UIColor(
		red		: CGFloat( ( p >> 16 ) & 0x0ff ) / 255
	,	green	: CGFloat( ( p >> 8 ) & 0x0ff ) / 255
	,	blue	: CGFloat( p & 0x0ff ) / 255
	,	alpha	: CGFloat( p >> 24 ) / 255
	)
}

class
JPFitLabel	: UIView {
	var
	text = "" {
		didSet {
			DispatchQueue.main.async{ self.setNeedsDisplay() }
		}
	}
	
	var
	font = UIFont.systemFont( ofSize: 17 ) {
		didSet {
			DispatchQueue.main.async{ self.setNeedsDisplay() }
		}
	}
	
	private	var
	attributes	: [ NSAttributedStringKey: Any ] {
		get { return [ .font: font, .foregroundColor: tintColor ] }
	}

	override func
	draw(_ rect: CGRect) {
		guard let wC = UIGraphicsGetCurrentContext() else { return }
		
		let	wBBox = text.boundingRect( with: CGSize.zero, options: [], attributes: attributes, context: nil )

		if wBBox.size.width > self.bounds.size.width {	//	SHRINK
			wC.scaleBy( x: self.bounds.size.width / wBBox.size.width, y: 1 )
			text.draw(
				at: CGPoint(
					x: self.bounds.origin.x
				,	y: ( self.bounds.size.height - wBBox.size.height ) / 2
				)
			,	withAttributes: attributes
			)
		} else {	//	CENTER
			text.draw(
				at: CGPoint(
					x: ( self.bounds.size.width - wBBox.size.width ) / 2
				,	y: ( self.bounds.size.height - wBBox.size.height ) / 2
				)
			,	withAttributes: attributes
			)
		}
	}
}

