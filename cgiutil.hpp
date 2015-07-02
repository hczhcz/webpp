#pragma once

#include <unordered_map>

#include <fcgio.h>
#include <cgicc/Cgicc.h>

namespace cgicc {

template <
    class Env = std::unordered_map<std::string, std::string>,
    class In = std::istream
>
class FCgiInput: public cgicc::CgiInput {
private:
    Env env;
    In &in;

public:
    FCgiInput(
        char *_env[],
        In &_in
    ): env{}, in{_in} {
        for (; *_env; ++_env) {
            char *begin = *_env;
            char *pos = begin;

            // find split
            for (; *pos; ++pos) {
                if (*pos == '=') {
                    break;
                }
            }

            if (*pos) {
                // '=' found
                *pos = '\0';
                env.insert({
                    std::string{begin},
                    std::string{pos + 1}
                });
            } else {
                // '=' not found
                throw std::exception{};
            }
        }
    }

    FCgiInput(const FCgiInput &obj): env{obj.env}, in{obj.in} {}
    FCgiInput(FCgiInput &&obj): env{std::move(obj.env)}, in{obj.in} {}

    virtual std::string getenv(const char *key) override {
        return env[key];
    }

    virtual size_t read(char *buf, size_t n) override {
        in.read(buf, n);

        return in.gcount();
    }

    operator In&() {
        return in;
    }
};

template <
    class Out = std::ostream
>
class FCgiOutput {
private:
    Out &out;

public:
    FCgiOutput(Out &_out): out{_out} {}
    FCgiOutput(const FCgiOutput &obj): out{obj.out} {}
    FCgiOutput(FCgiOutput &&obj): out{obj.out} {}

    operator Out&() {
        return out;
    }
};

template <
    class Env = std::unordered_map<std::string, std::string>,
    class In = std::istream,
    class Out = std::ostream,
    class Err = std::ostream
>
class FCgiIO {
private:
    fcgi_streambuf in_buf;
    fcgi_streambuf out_buf;
    fcgi_streambuf err_buf;

protected:
    FCgiInput<Env, In> input;
    FCgiOutput<Out> output;
    FCgiOutput<Err> error;

public:
    FCgiIO(
        FCGX_Request &req,
        In &_in,
        Out &_out,
        Err &_err
    ):
        in_buf{req.in}, out_buf{req.out}, err_buf{req.err},
        input{req.envp, _in}, output{_out}, error{_err}
    {
        _in.rdbuf(&in_buf);
        _out.rdbuf(&out_buf);
        _err.rdbuf(&err_buf);
    }
};

// CGICC main object with FastCGI integration
template <
    class Env = std::unordered_map<std::string, std::string>,
    class In = std::istream,
    class Out = std::ostream,
    class Err = std::ostream
>
class FCgiCC: public FCgiIO<Env, In, Out, Err>, public cgicc::Cgicc {
public:
    FCgiCC(
        FCGX_Request &req,
        In &_in,
        Out &_out,
        Err &_err
    ):
        FCgiIO<Env, In, Out, Err>{req, _in, _out, _err},
        cgicc::Cgicc{&this->input}
    {
        // nothing
    }

    In &s_in() {
        return this->input;
    }

    Out &s_out() {
        return this->output;
    }

    Err &s_err() {
        return this->error;
    }

    template <class T>
    auto operator>>(T obj) -> decltype(*this) {
        this->s_in() >> obj;
        return *this;
    }

    template <class T>
    auto operator<<(T obj) -> decltype(*this) {
        this->s_out() << obj;
        return *this;
    }

    auto operator>>(In &(*pf)(In &)) -> decltype(*this) {
        this->s_in() >> pf;
        return *this;
    }

    auto operator<<(Out &(*pf)(Out &)) -> decltype(*this) {
        this->s_out() << pf;
        return *this;
    }

    const std::string &operator[](const std::string &key) {
        static std::string result;

        if (!this->getElement(key)->isEmpty()) {
            result = this->getElement(key)->getStrippedValue();
            return result;
        } else {
            result = "";
            return result;
        }
    }

    const std::string &at(const std::string &key) {
        static std::string result;

        if (!this->getElement(key)->isEmpty()) {
            result = this->getElement(key)->getStrippedValue();
            return result;
        } else {
            throw std::exception{};
        }
    }
};

}

// execute a FastCGI (+CGICC) event loop
template <class Env, class In, class Out, class Err, class E>
void fcgicc_exec(
    void (&func)(cgicc::FCgiCC<Env, In, Out, Err> &),
    void (&except)(const E &)
) {
    FCGX_Request request;

    FCGX_Init();
    FCGX_InitRequest(&request, 0, 0);

    while (FCGX_Accept_r(&request) == 0) {
        try {
            cgicc::FCgiCC<Env, In, Out, Err> cgi{
                request, std::cin, std::cout, std::cerr // TODO: create new stream
            };

            func(cgi);
        } catch (const E &e) {
            except(e);
        }
    }
}
