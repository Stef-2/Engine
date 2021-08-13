#ifndef ANIMATION_H
#define ANIMATION_H


namespace Engine
{

    class Animation
    {
    public:
        Animation();

    private:
        float duration;
        unsigned int ticksPerSecond;
    };
}
#endif // ANIMATION_H
