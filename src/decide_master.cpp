// encoding: utf-8
// 大括号不换行的跟换行的打，变量名首字母大写的跟不大写的打，私有域前面加下划线的跟不加下划线的打

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <dolores/dolores.hpp>
#include "mt19937ar.h"

using namespace std;
using namespace cq;
using namespace dolores;
using namespace dolores::matchers;

typedef unsigned long long ull;

namespace decide {
    std::string get_context(ull uid, std::string arg) {
        vector<std::string> lst;
        ull pre = 0, l = arg.size();
        std::string res = "要让本机帮您选什么呢？";
        if (l) {
            for (register ull i = 0; i < l; ++i) {
                if (arg[i] == ' ') {
                    lst.push_back(arg.substr(pre, i - pre));
                    pre = i + 1;
                }
            }
            lst.push_back(arg.substr(pre, l - pre));
            l = lst.size();
            if (l == 1)
            res = "这样随便调戏本机就不好玩了，哼！";
            else if (l > 1) {
                ull t = time(NULL); // time
                ull mt_seed = ((t ^ 998244353) * uid) % 4294967295;
                init_genrand(mt_seed);
                for (register ull i = 1; i <= l; ++i) pre = genrand_int32() % l;
                res = "本机觉得应该是【" + lst[pre] + "】的说";
            }
        }
        return res;
    }
} // namespace decide

CQ_INIT {
    dolores::init();
    dolores::on_startup([] { 
        logging::info("准备加载", "在使用本插件前，请确保已经仔细阅读使用说明书，若未按照说明书使用，造成的一切后果与本插件作者无关。");
    });
}

CQ_MENU(menu_qwq) {
    logging::info("菜单", "真没什么可以调的东西");
}

dolores_on_message("二择_群", group(), to_me(command("decide"))) {
    const auto arg = current.command_argument();
    const auto &event = current.event_as<cq::GroupMessageEvent>();
    auto text = decide::get_context(event.user_id, arg);
    current.reply(text);
    current.event.block();
}

dolores_on_message("二择_讨论组", discuss(), to_me(command("decide"))) {
    const auto arg = current.command_argument();
    const auto &event = current.event_as<cq::DiscussMessageEvent>();
    auto text = decide::get_context(event.user_id, arg);
    current.reply(text);
    current.event.block();
}

dolores_on_message("二择_私", direct(), to_me(command("decide"))) {
    const auto arg = current.command_argument();
    const auto &event = current.event_as<cq::MessageEvent>();
    auto text = decide::get_context(event.user_id, arg);
    current.reply(text);
    current.event.block();
}