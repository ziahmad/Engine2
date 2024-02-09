#pragma once
#include <chrono>

/**
 * @brief This is the bones of an engine. It will handle objects and physics, updateing them, and sound
 * screen types (like console of gui) should be handled by calling the methid to draw the frame inside of loop
 */
namespace Engine2
{

    namespace TimeHandler
    {
        using clockType = std::chrono::steady_clock;
        using time_S = std::chrono::duration<double, std::ratio<1, 1>>;
        using time_MS = std::chrono::duration<double, std::milli>;
        using time_US = std::chrono::duration<double, std::micro>;
        using time_NS = std::chrono::duration<double, std::nano>;

        template <typename ToDuration, typename Rep, typename Period>
        ToDuration time_cast(const std::chrono::duration<Rep, Period> &d)
        {
            return std::chrono::duration_cast<ToDuration>(d);
        }

        class EngineTime
        {
        private:
            char *infoBuf = new char[128];
            int targetFPS;
            int targetUpdateRate;
            time_MS frameTime_ms;
            time_MS updateTime_ms;
            size_t frameCounter = 0;
            size_t updateCounter = 0;
            // time handling
            clockType::time_point curTime = clockType::now();
            clockType::time_point prevTime = curTime;
            clockType::time_point fprevTime = curTime;
            clockType::time_point uprevTime = curTime;

            time_MS timeAccu = time_MS(0);
            time_NS fdt;
            time_NS udt;
            time_NS rdt;
            time_S time = time_S(0);

        public:
            EngineTime()
            {
                targetFPS = 60;
                targetUpdateRate = 60;
                ChangeFPS(60);
                ChangeUpdateRate(60);
            }

            /**
             * @brief Construct a new Engine Time object
             *
             * @param fps -1 mis uncapped
             * @param updateRate  -1 is uncapped
             */
            EngineTime(int fps, int updateRate)
            {
                targetFPS = fps;
                targetUpdateRate = updateRate;
                ChangeFPS(fps);
                ChangeUpdateRate(updateRate);
            }

            ~EngineTime()
            {
                delete[] infoBuf;
            }

            /**
             * @brief resets everything to 0 except current time, whoch gets set to current time;
             *
             */
            void reset();

            time_NS getUDT();

            time_NS getFDT();

            time_NS getRDT();

            time_S getTime();

            /**
             * @brief updates time, udt, fdt, realtime dt, sets prev time to curtime, then updates current time
             * call at the tim of each loop
             */
            void update();

            /**
             * @brief updates the time at which the last frame was called
             *
             */
            void updateFrameTime();

            /**
             * @brief updates the time at which the last update was called
             *
             */
            void updateUpdateTime();

            /**
             * @brief increases time by update
             *
             */
            void incrTime();

            time_MS getTimeAccumulator();

            void resetTimeAccumulator();

            char *frameInfo();

            time_MS GetFrameTime() const;
            time_MS GetUpdateTime() const;

            void ChangeFPS(int fps);
            void ChangeUpdateRate(int ur);

            void ResetFrameCounter();
            void ResetUpdateCounter();

            void IncreaseFrameCounter();
            void IncreaseUpdateCounter();

            size_t GetframeCounter() const;
            size_t GetUpdateCounter() const;

            time_MS GetFPS() const;
            time_MS GetUpdateRate() const;
        };

    } // namespace TimeHandler
    class Engine2
    {
    private:
        static Engine2 *thisEng;

        TimeHandler::EngineTime engTime;

        bool doShowFrameInfo = false;

        Engine2(int fps, int ups);

    public:
        /**
         * @brief Makes a new engine if one is not already made
         *
         * @param fps sets fps for drawing to screen, -1 is uncapped
         * @param ups sets ups for updating time, -1 is uncapped
         * @return Engine2*
         */
        static Engine2 *makeEngine(int fps, int ups);

        //add function headers for get  time get dt and udt
        /**
         * @brief returns time in seconds
         *
         * @return const double
         */
        const double getTime();

        /**
         * @brief returns deltatime since last update in seconds

         *
         * @return const double
         */
        const double getUDT();

        /**
         * @brief returns deltatime since last frame in seconds

         *
         * @return const double
         */
        const double getFDT();

        



        /**
         * @brief starts the engine
         *
         */
        void start();

        /**
         * @brief Set if to show Show Frame Info
         *
         */
        void setShowFrameInfo(bool);

        ~Engine2();
    };
    namespace loop
    {
        namespace set
        {
            /**
             * @brief Set the FPS  -1 is uncapped
             *
             */
            void setFPS(int);

            /**
             * @brief sets weather or not to show frame info
             *
             */
            void showFrameInfo(bool);
        } // namespace setup

        namespace data
        {
            /**
             * @brief returns time in seconds
             *
             * @return const double
             */
            const double time();

            /**
             * @brief returns deltatime since last update in seconds

             *
             * @return const double
             */
            const double updateDT();

            /**
             * @brief returns deltatime since last frame in seconds

             *
             * @return const double
             */
            const double frameDT();
        }

        /**
         * @brief gets called at the start of engine to set up things
         *
         */
        extern void setup();

        /**
         * @brief gets called every update

         *
         */
        extern void update();

        /**
         * @brief gets called every frame
         *
         */
        extern void loop();
    } // namespace loopvoid setFPS(int);

}