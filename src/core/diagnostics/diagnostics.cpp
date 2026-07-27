#include "diagnostics.hpp"

namespace dan::core
{

Stream* Diagnostics::output_ = nullptr;
bool Diagnostics::enabled_ = false;

void Diagnostics::Initialize(Stream& output)
{
    output_ = &output;
    enabled_ = false;
}

void Diagnostics::Enable()
{
    enabled_ = true;
}

void Diagnostics::Disable()
{
    enabled_ = false;
}

bool Diagnostics::IsEnabled()
{
    return enabled_;
}

void Diagnostics::Print(const char* text)
{
    if (!enabled_ || output_ == nullptr || text == nullptr)
    {
        return;
    }

    output_->print(text);
}

void Diagnostics::PrintLine(const char* text)
{
    if (!enabled_ || output_ == nullptr || text == nullptr)
    {
        return;
    }

    output_->println(text);
}

} // namespace dan::core
