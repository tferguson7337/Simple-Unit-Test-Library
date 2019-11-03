#include <UnitTestResult.h>


/// Ctors \\\

UnitTestResult::UnitTestResult() noexcept :
    m_Result(ResultType::NotRun),
    m_FuncName(std::string()),
    m_FileName(std::string()),
    m_LineNum(0),
    m_ResultInfo(std::string()),
    m_TestDurationMicroseconds(0)
{ }

UnitTestResult::UnitTestResult(
    _In_ ResultType result,
    _In_ const char* func, _In_ const size_t& funcLen,
    _In_ const char* file, _In_ const size_t& fileLen, _In_ const uint32_t& line,
    _In_ const std::string& info
) :
    m_Result(result),
    m_FuncName(ExtractFuncName(func, funcLen)),
    m_FileName(ExtractFileName(file, fileLen)),
    m_LineNum(line),
    m_ResultInfo(info),
    m_TestDurationMicroseconds(0)
{ }

// Move Ctor
UnitTestResult::UnitTestResult(_Inout_ UnitTestResult&& src) noexcept :
    UnitTestResult()
{ 
    *this = std::move(src);
}

// Operator Overloads \\

UnitTestResult& UnitTestResult::operator=(_Inout_ UnitTestResult&& src) noexcept
{
    if (this != &src)
    {
        m_Result = src.m_Result;
        m_LineNum = src.m_LineNum;
        m_FuncName.assign(std::move(src.m_FuncName));
        m_FileName.assign(std::move(src.m_FileName));
        m_ResultInfo.assign(std::move(src.m_ResultInfo));
        m_TestDurationMicroseconds = src.m_TestDurationMicroseconds;

        src.Clear();
    }

    return *this;
}

UnitTestResult::operator bool() const noexcept
{
    return m_Result == ResultType::Success;
}

// Private Helpers \\

constexpr const char* UnitTestResult::ExtractNameCommon(_In_ const char* str, _In_ const size_t& len, _In_ ExtractPredicate pred) noexcept
{
    if (!str || len == 0)
    {
        return nullptr;
    }

    const char* name = str + len;
    while (name >= str)
    {
        if (pred(*(--name), m_LTGTCounter) && m_LTGTCounter == 0)
        {
            break;
        }
    }

    return name + sizeof(char);
}

constexpr const char* UnitTestResult::ExtractFileName(_In_ const char* f, _In_ const size_t& n) noexcept
{
    const char* p = ExtractNameCommon(f, n, IsPathSeparator);
    return p ? p : "<BAD_FILE_NAME>";
}

constexpr const char* UnitTestResult::ExtractFuncName(_In_ const char* f, _In_ const size_t& n) noexcept
{
    const char* p = ExtractNameCommon(f, n, IsWhitespace);
    return p ? p : "<BAD_FUNC_NAME>";
}

constexpr bool UnitTestResult::IsPathSeparator(_In_ const char& c, _Inout_ uint32_t&) noexcept
{
    return (c == '/' || c == '\\');
}

constexpr bool UnitTestResult::IsWhitespace(_In_ const char& c, _Inout_ uint32_t& counter) noexcept
{
    if (c == '>')
    {
        counter++;
    }
    else if (c == '<')
    {
        counter--;
    }

    return (c == ' ');
}


// Getters \\

const ResultType& UnitTestResult::GetResult() const noexcept
{
    return m_Result;
}

const std::string& UnitTestResult::GetFunctionName() const noexcept
{
    return m_FuncName;
}

const std::string& UnitTestResult::GetFileName() const noexcept
{
    return m_FileName;
}

const uint32_t& UnitTestResult::GetLineNumber() const noexcept
{
    return m_LineNum;
}

const std::string& UnitTestResult::GetResultInfo() const noexcept
{
    static const std::string unknownStr("<NULL>");
    return (m_ResultInfo.empty()) ? unknownStr : m_ResultInfo;
}

const uint64_t& UnitTestResult::GetTestDurationMicroseconds() const noexcept
{
    return m_TestDurationMicroseconds;
}


// Setters \\

void UnitTestResult::SetResult(_In_ const ResultType& res) noexcept
{
    m_Result = res;
}

void UnitTestResult::SetFunctionName(_In_ const std::string& func)
{
    m_FuncName = func;
}

void UnitTestResult::SetFileName(_In_ const std::string& file)
{
    m_FileName = file;
}

void UnitTestResult::SetLineNumber(_In_ const uint32_t& line) noexcept
{
    m_LineNum = line;
}

void UnitTestResult::SetResultInfo(_In_ const std::string& info)
{
    m_ResultInfo = info;
}

void UnitTestResult::SetTestDurationMicroseconds(_In_ const std::chrono::duration<int64_t, std::micro>& dur) const noexcept
{
    m_TestDurationMicroseconds = (dur.count() > 0) ? static_cast<uint64_t>(dur.count()) : 0;
}


// Public Methods \\

void UnitTestResult::Clear() noexcept
{
    m_Result = ResultType::NotRun;
    m_LineNum = 0;
    m_FuncName.clear();
    m_FileName.clear();
    m_ResultInfo.clear();
    m_TestDurationMicroseconds = 0;
}
