#ifndef _TREELS_APP_

struct TreelsApplication : public Treels::TreelsEngine {
public:
    void setup() override;
    void run() override;

    void closing() override;
    void resize(float newWidth, float newHeight) override;
    void keyPress(Treels::Action action, Treels::Key key) override;
};

#endif // !_TREELS_APP_
