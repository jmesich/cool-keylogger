#ifndef TIMER_H
    #define TIMER_H

    #include<thread>
    #include<chrono>
    class Timer
    {
        std::thread Thread;
        bool Alive = false;
        long CallNumber=-1L;
        long repeat_Count=-1L;
        std::chrono::milliseconds interval = std::chrono::milliseconds(0);
        std::function<void(void)> funct = nullptr;

        void SleepAndRun()
        {
            std::this_thread::sleep_for(interval);
            if(Alive)
            {
                Function()();
            }
        }

        void ThreadFunc()
        {
            if(CallNumber==Infinite)
            {
                while(Alive)
                {
                    SleepAndRun();
                }
            }
            else
            {
                while(repeat_Count--)
                {
                    SleepAndRun();
                }
            }
        }
    public:
        static const long Infinite = -1L;

        Timer(){}

        Timer(const std::function <void(void)> &f): funct (f) {}
        Timer(const std::function <void(void)> &f,
              const unsigned long &i,
              const long repeat = Timer::Infinite): funct (f),
                                                    interval(std::chrono::milliseconds(i)),
                                                    CallNumber(repeat){}
        void Start(bool async = true)
        {
            if(isAlive())
            {
                    return;
            }
            Alive = true;
            repeat_Count=CallNumber;
            if(async)
            {
                Thread= std::thread(ThreadFunc,this);
            }
            else
            {
                this->ThreadFunc();
            }
        }
        void Stop()
        {
            Alive=false;
            Thread.join();
        }
        void SetFunction(const std::function<void(void)> &f)
        {
            funct=f;
        }
        bool isAlive() const
        {
            return Alive;
        }
        void RepeatCount(const long r)
        {
            if(Alive) return;
            CallNumber = r;
        }
        long GetLeft() const
        {
            return repeat_Count;
        }
        long RepeatCount() const
        {
            return CallNumber;
        }
        void setInterval(const unsigned long &i)
        {
            if(Alive)return;
            interval = std::chrono::milliseconds(i);
        }
        long unsigned Interval() const
        {
            return interval.count();
        }
        const std::function<void(void)> &Function() const
        {
            return funct;

        }
    };
#endif // TIMER_H
