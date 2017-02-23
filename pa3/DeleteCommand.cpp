#include "DeleteCommand.h"


DeleteCommand::DeleteCommand(const wxPoint& start, std::shared_ptr<Shape> shape)
	: Command(start, shape)
{
}


DeleteCommand::~DeleteCommand()
{
}

void DeleteCommand::Finalize(std::shared_ptr<PaintModel> model)
{
	mShape = model->GetSelectedShape();
	model->AddCurrentCommandToUndoStack();
	model->RemoveShape(model->GetSelectedShape());
	model->GetSelectedShape().reset();
	model->GetCurrentCommand().reset();
}

void DeleteCommand::Undo(std::shared_ptr<PaintModel> model)
{
	// Add shape and undo stack
//	model->UndoShape(); // see if ti works without this
	model->AddShape(mShape);
;	model->Undo();
}

void DeleteCommand::Redo(std::shared_ptr<PaintModel> model)
{
	// Remove shape and redo stack
//	model->RedoShape(); // see if ti works without this
	model->RemoveShape(mShape);
	model->Redo();
}
