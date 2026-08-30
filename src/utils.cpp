#include "utils.h"
#include "string.h"


void GTimerManager::update(float delta_time) {
    vector<GTimer*> done;
    m_timers.for_each([&](GTimer& timer) {
        timer.elapsed_time += delta_time;
        if (timer.elapsed_time >= timer.lifetime or !timer.only_call_at_end) {
            (timer.on_update)(timer);
        }
        if (timer.elapsed_time >= timer.lifetime) {
            done.push_back(&timer);
        }
        });
    for (GTimer* i : done) {
        m_timers.destroy(i);
    }
}

void GTimerManager::drop_timer(GTimer* timer) {
    m_timers.destroy(timer);
}

GTimer* GTimerManager::new_timer(GTimer time) {
    return m_timers.create(time);
}
GTimer* GTimerManager::timer(function<void(GTimer&)> to_run, float time) {
    GTimer timer;
    timer.on_update = to_run;
    timer.elapsed_time = 0.0;
    timer.only_call_at_end = true;
    timer.lifetime = time;
    return this->new_timer(timer);
}

GTimer* GTimerManager::repeating_timer(function<void(GTimer&)> to_run, float time) {
    GTimer timer;
    timer.on_update = to_run;
    timer.elapsed_time = 0.0;
    timer.only_call_at_end = false;
    timer.lifetime = time;
    return this->new_timer(timer);
}