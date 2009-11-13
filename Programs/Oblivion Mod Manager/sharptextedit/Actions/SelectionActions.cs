// <file>
//     <copyright see="prj:///doc/copyright.txt"/>
//     <license see="prj:///doc/license.txt"/>
//     <owner name="Mike Krüger" email="mike@icsharpcode.net"/>
//     <version>$Revision: 1965 $</version>
// </file>

using System;
using System.Drawing;
using ICSharpCode.TextEditor.Document;

namespace ICSharpCode.TextEditor.Actions
{
	internal class ShiftCaretRight : CaretRight
	{
		public override void Execute(TextArea textArea)
		{
			Point oldCaretPos  = textArea.Caret.Position;
			base.Execute(textArea);
			textArea.AutoClearSelection = false;
			textArea.SelectionManager.ExtendSelection(oldCaretPos, textArea.Caret.Position);
		}
	}
	
	internal class ShiftCaretLeft : CaretLeft
	{
		public override void Execute(TextArea textArea)
		{
			Point oldCaretPos  = textArea.Caret.Position;
			base.Execute(textArea);
			textArea.AutoClearSelection = false;
			textArea.SelectionManager.ExtendSelection(oldCaretPos, textArea.Caret.Position);
		}
	}
	
	internal class ShiftCaretUp : CaretUp
	{
		public override void Execute(TextArea textArea)
		{
			Point oldCaretPos  = textArea.Caret.Position;
			base.Execute(textArea);
			textArea.AutoClearSelection = false;
			textArea.SelectionManager.ExtendSelection(oldCaretPos, textArea.Caret.Position);
		}
	}
	
	internal class ShiftCaretDown : CaretDown
	{
		public override void Execute(TextArea textArea)
		{
			Point oldCaretPos  = textArea.Caret.Position;
			base.Execute(textArea);
			textArea.AutoClearSelection = false;
			textArea.SelectionManager.ExtendSelection(oldCaretPos, textArea.Caret.Position);
		}
	}
	
	internal class ShiftWordRight : WordRight
	{
		public override void Execute(TextArea textArea)
		{
			Point oldCaretPos  = textArea.Caret.Position;
			base.Execute(textArea);
			textArea.AutoClearSelection = false;
			textArea.SelectionManager.ExtendSelection(oldCaretPos, textArea.Caret.Position);
		}
	}
	
	internal class ShiftWordLeft : WordLeft
	{
		public override void Execute(TextArea textArea)
		{
			Point oldCaretPos  = textArea.Caret.Position;
			base.Execute(textArea);
			textArea.AutoClearSelection = false;
			textArea.SelectionManager.ExtendSelection(oldCaretPos, textArea.Caret.Position);
		}
	}
	
	internal class ShiftHome : Home
	{
		public override void Execute(TextArea textArea)
		{
			Point oldCaretPos  = textArea.Caret.Position;
			base.Execute(textArea);
			textArea.AutoClearSelection = false;
			textArea.SelectionManager.ExtendSelection(oldCaretPos, textArea.Caret.Position);
		}
	}
	
	internal class ShiftEnd : End
	{
		public override void Execute(TextArea textArea)
		{
			Point oldCaretPos  = textArea.Caret.Position;
			base.Execute(textArea);
			textArea.AutoClearSelection = false;
			textArea.SelectionManager.ExtendSelection(oldCaretPos, textArea.Caret.Position);
		}
	}
	
	internal class ShiftMoveToStart : MoveToStart
	{
		public override void Execute(TextArea textArea)
		{
			Point oldCaretPos  = textArea.Caret.Position;
			base.Execute(textArea);
			textArea.AutoClearSelection = false;
			textArea.SelectionManager.ExtendSelection(oldCaretPos, textArea.Caret.Position);
		}
	}
	
	internal class ShiftMoveToEnd : MoveToEnd
	{
		public override void Execute(TextArea textArea)
		{
			Point oldCaretPos  = textArea.Caret.Position;
			base.Execute(textArea);
			textArea.AutoClearSelection = false;
			textArea.SelectionManager.ExtendSelection(oldCaretPos, textArea.Caret.Position);
		}
	}
	
	internal class ShiftMovePageUp : MovePageUp
	{
		public override void Execute(TextArea textArea)
		{
			Point oldCaretPos  = textArea.Caret.Position;
			base.Execute(textArea);
			textArea.AutoClearSelection = false;
			textArea.SelectionManager.ExtendSelection(oldCaretPos, textArea.Caret.Position);
		}
	}
	
	internal class ShiftMovePageDown : MovePageDown
	{
		public override void Execute(TextArea textArea)
		{
			Point oldCaretPos  = textArea.Caret.Position;
			base.Execute(textArea);
			textArea.AutoClearSelection = false;
			textArea.SelectionManager.ExtendSelection(oldCaretPos, textArea.Caret.Position);
		}
	}
	
	internal class SelectWholeDocument : AbstractEditAction
	{
		public override void Execute(TextArea textArea)
		{
			textArea.AutoClearSelection = false;
			Point startPoint = new Point(0, 0);
			Point endPoint   = textArea.Document.OffsetToPosition(textArea.Document.TextLength);
			if (textArea.SelectionManager.HasSomethingSelected) {
				if (textArea.SelectionManager.SelectionCollection[0].StartPosition == startPoint &&
				    textArea.SelectionManager.SelectionCollection[0].EndPosition   == endPoint) {
					return;
				}
			}
			textArea.SelectionManager.SetSelection(new DefaultSelection(textArea.Document, startPoint, endPoint));
		}
	}
	
	internal class ClearAllSelections : AbstractEditAction
	{
		public override void Execute(TextArea textArea)
		{
			textArea.SelectionManager.ClearSelection();
		}
	}
}
