
#ifndef OBSERVER_H
#define OBSERVER_H

#include <functional>
#include <list>

namespace rbtree {
template<class T>
class Observer;

template<class T>
class Observable;

template<class T>
class Observable
{
    using Observer = Observer<T>;
    friend Observer;

public:
    Observable(T &&data)
        : data_(std::move(data))
    {}
    Observable(const Observable &) = delete;
    Observable &operator=(const Observable &) = delete;
    Observable(Observable &&) noexcept = delete;
    Observable &operator=(Observable &&) noexcept = delete;
    ~Observable()
    {
        while (!Observers_.empty()) {
            Observers_.front()->unsubscribe();
        }
    }

    void subscribe(Observer *obs)
    {
        if (obs->isSubscribed())
            obs->unsubscribe();
        Observers_.push_back(obs);
        obs->setObservable_(this);
        obs->onNotify_(data_);
    }
    void notify() const
    {
        for (auto obs : Observers_)
            obs->onNotify_(data_);
    }

    void set(const T &data)
    {
        data_ = std::move(data);
        notify();
    }

private:
    void detach_(Observer *obs) { Observers_.remove(obs); }

    T data_;
    std::list<Observer *> Observers_;
};

template<class T>
class Observer
{
    using Observable = Observable<T>;
    friend Observable;

public:
    template<class Tt>
    Observer(Tt &&onNotify)
        : onNotify_(std::forward<Tt>(onNotify))
    {}
    Observer(const Observer &) = delete;
    Observer &operator=(const Observer &) = delete;
    Observer(Observer &&) noexcept = delete;
    Observer &operator=(Observer &&) noexcept = delete;
    ~Observer() { unsubscribe(); }

    void unsubscribe()
    {
        if (!isSubscribed())
            return;
        Observable_->detach_(this);
        Observable_ = nullptr;
    }
    bool isSubscribed() const { return Observable_; }

private:
    void setObservable_(Observable *observable) { Observable_ = observable; }
    Observable *Observable_ = nullptr;
    std::function<void(const T &)> onNotify_;
};
} // namespace rbtree

#endif // OBSERVER_H
