#ifndef IOBSERVER_HPP
#define IOBSERVER_HPP

template <typename TType>
class IObserver
{
private:
public:
    virtual ~IObserver()                       = default;
    virtual void update(const TType& newValue) = 0;
};

#endif