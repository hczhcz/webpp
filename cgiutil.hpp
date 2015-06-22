#pragma once

#include <unordered_map>

#include "fcgio.h"
#include "cgicc/Cgicc.h"

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
                env.insert({
                    std::string{begin, static_cast<size_t>(pos - begin)},
                    std::string{pos + 1}
                });
            } else {
                // '=' not found
                env.insert({
                    begin,
                    ""
                });
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
    Out &out;
    Err &err;

public:
    FCgiIO(
        FCGX_Request &req,
        In &_in,
        Out &_out,
        Err &_err
    ):
        in_buf{req.in}, out_buf{req.out}, err_buf{req.err},
        input{req.envp, _in}, out{_out}, err{_err}
    {
        _in.rdbuf(&in_buf);
        _out.rdbuf(&out_buf);
        _err.rdbuf(&err_buf);
    }
};

template <
    class Env = std::unordered_map<std::string, std::string>,
    class In = std::istream,
    class Out = std::ostream,
    class Err = std::ostream
>
class FCgiCC: public FCgiIO<Env, In, Out, Err>, public cgicc::Cgicc {
protected:
    using FCgiIO<Env, In, Out, Err>::input;

public:
    FCgiCC(
        FCGX_Request &req,
        In &_in,
        Out &_out,
        Err &_err
    ):
        FCgiIO<Env, In, Out, Err>{req, _in, _out, _err},
        cgicc::Cgicc{&input}
    {
        // nothing
    }
};
