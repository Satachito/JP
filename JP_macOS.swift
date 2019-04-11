import Cocoa

func
Warning( _ pMessage: String, _ pInfomative: String = "" ) {
	let	w = NSAlert()
	w.messageText = pMessage
	w.informativeText = pInfomative
	w.alertStyle = .warning
	w.addButton( withTitle: "OK" )
	w.runModal()
}

func
Error( _ p: Error ) {
	let	w = NSAlert()
	w.messageText = "Error"
	w.informativeText = p.localizedDescription
	w.alertStyle = .critical
	w.addButton( withTitle: "OK" )
	w.runModal()
}

func
Input(
	_ pView: NSView
,	_ pMessage: String = ""
,	_ pInfomative: String = ""
) -> Bool {
	let	w = NSAlert()
	w.messageText = pMessage
	w.informativeText = pInfomative
	w.alertStyle = .informational
	w.addButton( withTitle: "OK" )
	w.addButton( withTitle: "Cancel" )
    w.accessoryView = pView
	return w.runModal() == .alertFirstButtonReturn
}

func
OKCancel( _ pMessage: String, _ pInfomative: String,_ ed: () -> () ) {
	let	w = NSAlert()
	w.messageText = pMessage
	w.informativeText = pInfomative
	w.alertStyle = .warning
	w.addButton( withTitle: "OK" )
	w.addButton( withTitle: "Cancel" )
	if w.runModal() == .alertFirstButtonReturn { ed() }
}

func
YesNo( _ pMessage: String, _ pInfomative: String, _ yesed: () -> (), _ noed: () -> () ) {
	let	w = NSAlert()
	w.messageText = pMessage
	w.informativeText = pInfomative
	w.alertStyle = .warning
	w.addButton( withTitle: "OK" )
	w.addButton( withTitle: "Cancel" )
	switch w.runModal() {
	case .alertFirstButtonReturn	: yesed()
	case .alertSecondButtonReturn	: noed()
	default							: break
	}
}

class
V	: NSView {
	var drawer	: ( CGRect ) -> () = { _ in }
	override func
	draw( _ p: CGRect ) {
		super.draw( p )
		drawer( p )
	}
}

