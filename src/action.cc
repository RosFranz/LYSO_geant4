#include "action.hh"

MyActionInitialization::MyActionInitialization()
{}

MyActionInitialization::~MyActionInitialization()
{}

//MULTITHREADING
/*void MyActionInitialization::BuildForMaster() const
{
    MyRunAction *runAction = new MyRunAction();
    SetUserAction(runAction);
}*/

void MyActionInitialization::Build() const
{
    MyPrimaryGenerator *generator = new MyPrimaryGenerator();
    SetUserAction(generator);

    MyRunAction *runAction = new MyRunAction();
    SetUserAction(runAction);

    MyEventAction *eventAction = new MyEventAction(runAction);
    SetUserAction(eventAction);
    
    MySteppingAction *steppingAction = new MySteppingAction(eventAction);
    SetUserAction(steppingAction);
}