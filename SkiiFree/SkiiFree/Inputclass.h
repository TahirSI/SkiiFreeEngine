class Inputclass
{
public:
	Inputclass();
	Inputclass(const Inputclass&);
	~Inputclass();

	void initialize();

	void keyDown(unsigned int);
	void keyUp(unsigned int);

	bool isKeyDown(unsigned int);

private:
	bool keys[256];
};