struct Page{
    char buffer[32];
    double time;
};
class displayDriver
{
    int _a;
    public:
        void setvar  (int);
        int getvar  ();
};

int displayDriver::getvar  ()
{
    return _a;
}
void displayDriver::setvar  (int a)
{
    _a = a;
}