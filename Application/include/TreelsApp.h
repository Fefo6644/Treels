#ifndef _TREELSAPP_H_
#define _TREELSAPP_H_

struct TreelsApplication {
	/* Mandatory functions */
	TreelsApplication(Treels::TreelsEngine* applicationWindow);
	void TreelsRun();
	static size_t GetStructSize();

	/* Optional event functions */
	Treels::TreelsEngine* window = nullptr;

	void KeyPress(Treels::Action action, Treels::Key key);
	void Resize(float newWidth, float newHeight);
	void Closing();

	/* Application specific stuff */
	bool reset = false;
	bool inc = false;
	bool dec = false;
	bool shifting = false;
	bool speeding = false;
	float multiplier = 0.0f;
	float windowWidth = 0.0f;
	float windowHeight = 0.0f;

	Treels::Objects::Point center = Treels::Objects::Point(0.0f, 0.0f);
	Treels::Objects::Color lineColor = Treels::Objects::Color(0.0f, 0.0f, 0.0f, 1.0f);
	::std::vector<Treels::Objects::Line>* pLines = new ::std::vector<Treels::Objects::Line>;
	::std::vector<Treels::Objects::Circle>* pBalls = new ::std::vector<Treels::Objects::Circle>;
};

#endif // !_TREELSAPP_H_
