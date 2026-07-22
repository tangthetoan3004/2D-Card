#pragma once
#include <vector>
#include <memory>
#include "Command.h"

class CommandHistory
{
public:
    CommandHistory() = default;
    ~CommandHistory() = default;

    void PushCommand(std::unique_ptr<Command> cmd);
    bool Undo();
    bool Redo();
    void Clear();

    bool CanUndo() const;
    bool CanRedo() const;

private:
    std::vector<std::unique_ptr<Command>> mUndoStack;
    std::vector<std::unique_ptr<Command>> mRedoStack;
};
