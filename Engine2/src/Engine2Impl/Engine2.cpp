#include "Engine2/Engine2.hpp"

// implement all functions
namespace Engine2::TimeHandler
{

    void EngineTime::reset()
    {
        frameCounter = 0;
        updateCounter = 0;
        // time handling
        curTime = clockType::now();
        prevTime = curTime;
        fprevTime = curTime;
        uprevTime = curTime;

        timeAccu = time_MS(0);
        time = time_S(0);
    }

    void EngineTime::update()
    {
        prevTime = curTime;
        curTime = clockType::now();
        time += time_cast<time_S>(curTime - prevTime);
        timeAccu += time_cast<time_MS>(curTime - prevTime);
        rdt = time_cast<time_NS>(curTime - prevTime);
        udt = time_cast<time_NS>(curTime - uprevTime);
        fdt = time_cast<time_NS>(curTime - fprevTime);
    }

    time_S EngineTime::getTime()
    {
        return time;
    }

    void EngineTime::resetTimeAccumulator()
    {
        timeAccu = time_MS(0);
    }

    time_NS EngineTime::getUDT()
    {
        return udt;
    }
    time_NS EngineTime::getFDT()
    {
        return fdt;
    }

    time_NS EngineTime::getRDT()
    {
        return rdt;
    }

    void EngineTime::updateFrameTime()
    {
        fprevTime = curTime;
    }

    void EngineTime::updateUpdateTime()
    {
        uprevTime = curTime;
    }

    void EngineTime::incrTime()
    {
        time += udt;
    }

    time_MS EngineTime::getTimeAccumulator()
    {
        return timeAccu;
    }

    char *EngineTime::frameInfo()
    {
        // fill null
        memset(infoBuf, 0, 128);
        sprintf(infoBuf, "FPS: %.2f, UPS: %.2f", GetFPS(), GetUpdateRate());
        return infoBuf;
    }

    time_MS EngineTime::GetFrameTime() const
    {
        return frameTime_ms;
    }
    time_MS EngineTime::GetUpdateTime() const
    {
        return updateTime_ms;
    }
    void EngineTime::ChangeFPS(int fps)
    {
        if (fps > 0)
        {
            frameTime_ms = time_MS(1000.0 / fps);
        }
        else
        {
            frameTime_ms = time_MS(-1);
        }
    }
    void EngineTime::ChangeUpdateRate(int ur)
    {
        if (ur > 0)
        {
            updateTime_ms = time_MS(1000.0 / ur);
        }
        else
        {
            updateTime_ms = time_MS(-1);
        }
    }
    void EngineTime::ResetFrameCounter()
    {
        frameCounter = 0;
    }
    void EngineTime::ResetUpdateCounter()
    {
        updateCounter = 0;
    }
    void EngineTime::IncreaseFrameCounter()
    {
        frameCounter++;
    }
    void EngineTime::IncreaseUpdateCounter()
    {
        updateCounter++;
    }
    size_t EngineTime::GetframeCounter() const
    {
        return frameCounter;
    }
    size_t EngineTime::GetUpdateCounter() const
    {
        return updateCounter;
    }
    time_MS EngineTime::GetFPS() const
    {
        return frameTime_ms;
    }
    time_MS EngineTime::GetUpdateRate() const
    {

        return updateTime_ms;
    }
} // namespace Engine2::TimeHandler

namespace Engine2
{
    static Engine2 *minEng;

    void Engine2::setShowFrameInfo(bool b)
    {
        doShowFrameInfo = b;
    }

    Engine2::Engine2(int fps, int ups)
    {
        engTime = TimeHandler::EngineTime(fps, ups);
    }
    Engine2 *Engine2::makeEngine(int fps, int ups)
    {
        if (thisEng == nullptr)
        {
            thisEng = new Engine2(fps, ups);
            minEng = thisEng;
            return thisEng;
        }
        else
            return thisEng;
    }
    Engine2::~Engine2()
    {
        minEng = nullptr;
        delete thisEng;
        thisEng = nullptr;
    }

    void Engine2::start()
    {
        this->engTime.reset();
        loop::setup();

        while (true)
        {
            this->engTime.update();

            if (this->engTime.getFDT() > this->engTime.GetUpdateTime())
            {
                this->engTime.incrTime();
                loop::update();

                this->engTime.IncreaseUpdateCounter();
                this->engTime.updateUpdateTime();
            }

            if (this->engTime.getFDT() > this->engTime.GetFrameTime())
            {
                loop::loop();

                this->engTime.IncreaseFrameCounter();
                this->engTime.updateFrameTime();
            }

            if (this->engTime.getTimeAccumulator() >= TimeHandler::time_S(1))
            {
                if (doShowFrameInfo)
                {
                    std::string buf = this->engTime.frameInfo();
                    fwrite(buf.c_str(), buf.length(), 1, stdout);
                }
                this->engTime.ResetFrameCounter();
                this->engTime.ResetUpdateCounter();
                this->engTime.resetTimeAccumulator();
            }
        }
    }

    const double Engine2::getTime()
    {
        return this->engTime.getTime().count();
    }

    const double Engine2::getUDT()
    {
        return this->engTime.getUDT().count();
    }
    const double Engine2::getFDT()
    {
        return this->engTime.getFDT().count();
    }

} // namespace Engine2

namespace Engine2::loop
{
    void set::setFPS(int fps)
    {
    }
    // implemetn ::loop funcions
    namespace data
    {
        const double time()
        {
            return minEng->getTime();
        }
        const double updateDT()
        {
            return minEng->getUDT();
        }
        const double frameDT()
        {
            return minEng->getFDT();
        }
    }
}