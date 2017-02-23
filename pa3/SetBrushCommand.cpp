#include "SetBrushCommand.h"



SetBrushCommand::SetBrushCommand(const wxPoint& start, std::shared_ptr<Shape> shape)
	: Command(start, shape)
{
}


SetBrushCommand::~SetBrushCommand()
{
}

void SetBrushCommand::Update(const wxPoint& newPoint)
{
}

void SetBrushCommand::Finalize(std::shared_ptr<PaintModel> model)
{
//	model->AddCurrentBrushToBrushUndoStack();
	// Brush is added to the BrushUndoStack in PaintModel before the command is created
	model->AddCurrentCommandToUndoStack();
	model->GetCurrentCommand().reset();

	if (model->GetSelectedShape())
	{
		// add to undo for brush
		mShape = model->GetSelectedShape();
		mShape->SetBrushColor(model->GetBrush().GetColour());
	}
}

void SetBrushCommand::Undo(std::shared_ptr<PaintModel> model)
{
	model->Undo();
	mShape->SetBrushColor(model->GetTopInBrushUndo().GetColour());
	model->UndoBrush();
}

void SetBrushCommand::Redo(std::shared_ptr<PaintModel> model)
{
	model->Redo();
	mShape->SetBrushColor(model->GetTopInBrushRedo().GetColour());
	model->RedoBrush();
}

//void SetBrushCommand::ClearStacks()
//{
//	// Because C++ is stupid
//	while (!mUndoBrush.empty())
//	{
//		mUndoBrush.pop();
//	}
//	// Because C++ is stupid
//	while (!mRedoBrush.empty())
//	{
//		mRedoBrush.pop();
//	}
//}
