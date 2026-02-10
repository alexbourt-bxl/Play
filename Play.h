#ifndef PLAY_H
#define PLAY_H

#include <Controls\Control.h>
#include <Interface\PlayWindow\PlayWindow.h>
#include <Operators\Tree\Tree.h>

//#include "Types\Type.h"
//#include "Types\Basic\String.h"

int Play();

PlayWindow& GetMainWindow();
void opButton_OnPress(Control::Message& msg);

Tree& GetMainTree();

//void OnAddOperator(Container::Message& msg);

//void PrintTypeChain(Type type);
//void PrintFileParts(const String& filename);

#endif