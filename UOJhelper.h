// by Sshwy
#ifndef UOJ_HELPER_H
#define UOJ_HELPER_H

#include<vector>
#include<string>
#include<cstdlib>
#include<cassert>
#include<iostream>
using std::cout;
using std::endl;

using std::string;
using std::vector;
using std::pair;
using std::__cxx11::to_string;

namespace UOJ{
    char BUF[1048576];
}
class UOJ_problem{
    string name, std, val, chk;
    vector< pair<int,string> > tests, samples; // 0: text; 1: script
    vector< pair<int,int> > subtask;
    int ol, tl, ml, has_std, has_val, has_chk;
    public:
    UOJ_problem(string _name="prob", int _tl=1, int _ml=256):
        name(_name), tl(_tl), ml(_ml) { 
            has_std = has_val = has_chk = false, ol = 64;
        }
    void addTextTest(string test){
        tests.push_back({0,test});
    }
    void addScriptTest(string test){
        tests.push_back({1,test});
    }
    void addTextSample(string test){
        samples.push_back({0,test});
    }
    void addScriptSample(string test){
        samples.push_back({1,test});
    }
    template<typename... Targs>
    void addTextTest(const char * fmt, Targs ... args){
        sprintf(UOJ::BUF,fmt,args...);
        addTextTest(string(UOJ::BUF));
    }
    template<typename... Targs>
    void addScriptTest(const char * fmt, Targs ... args){
        sprintf(UOJ::BUF,fmt,args...);
        addScriptTest(string(UOJ::BUF));
    }
    template<typename... Targs>
    void addTextSample(const char * fmt, Targs ... args){
        sprintf(UOJ::BUF,fmt,args...);
        addTextSample(string(UOJ::BUF));
    }
    template<typename... Targs>
    void addScriptSample(const char * fmt, Targs ... args){
        sprintf(UOJ::BUF,fmt,args...);
        addScriptSample(string(UOJ::BUF));
    }
    void setTimeLimit(int _tl){
        tl=_tl;
    }
    void setMemoryLimit(int _ml){
        ml=_ml;
    }
    void setOutputLimit(int _ol){
        ol=_ol;
    }
    void setSubtask(int score){
        subtask.push_back({tests.size(),score});
    }
    void registerStd(string path){
        std = path, has_std = 1;
    }
    void registerValidator(string path){
        val = path, has_val = 1;
    }
    void registerChecker(string path){
        assert(0); // waiting for developing
        chk = path, has_chk = 2;
    }
    void setBuildinChecker(string s){
        // ncmp: （单行整数序列）比较有序64位整数序列
        // wcmp: （单行字符串序列）比较字符串序列
        // fcmp: （多行数据）逐行进行全文比较，不忽略行末空格，忽略文末回车。
        // yesno: 比较单个 YES 和 NO
        chk = s, has_chk = 1;
    }
    int fsystem(string command){
        return system(command.c_str());
    }
    template<typename... Targs>
    int fsystem(const char * fmt, Targs ... args){
        return system(fmt,args...);
    }
    void build(string path){
        if(!has_std || !has_chk) assert(0);
        if(path.back() != '/') path += "/";
        if(fsystem("mkdir --parents "+path)) exit(1);
        for(unsigned i=0;i<tests.size(); i++){
            printf("Test #%d\n",i+1);
            string fname = path+name+to_string(i+1);
            if(tests[i].first == 0){
                FILE * f = fopen((fname+".in").c_str(),"w");
                fprintf(f,"%s",tests[i].second.c_str());
                fclose(f);
            }else {
                if(fsystem(tests[i].second + " > " + fname + ".in"))assert(0);
            }
            if(has_val) if(fsystem(val + " < " + fname + ".in"))assert(0);
            if(fsystem("{ /usr/bin/time -f \"Test #" + to_string(i+1)
                        + ": real %es (user %Us sys %Ss)\" " 
                        + std + " < " + fname + ".in > " 
                        + fname + ".out; }"
                        + " 2>> " + path + "runtime.log"))
                assert(0);
        }
        for(unsigned i=0;i<samples.size(); i++){
            printf("Sample #%d\n",i+1);
            string fname = path+"ex_"+name+to_string(i+1);
            if(samples[i].first == 0){
                FILE * f = fopen((fname+".in").c_str(),"w");
                fprintf(f,"%s",samples[i].second.c_str());
                fclose(f);
            }else {
                if(fsystem(samples[i].second + " > " + fname + ".in"))assert(0);
            }
            if(has_val) if(fsystem(val + " < " + fname + ".in"))assert(0);
            if(fsystem("{ /usr/bin/time -f \"Sample #" + to_string(i+1) 
                        + ": real %es (user %Us sys %Ss)\" " 
                        + std + " < " + fname + ".in > " 
                        + fname + ".out; }"
                        + " 2>> " + path + "runtime.log"))
                assert(0);
        }

        FILE * config = fopen((path+"problem.conf").c_str(),"w");
        fprintf(config,"n_tests %lu\n",tests.size());
        fprintf(config,"n_ex_tests %lu\n",samples.size());
        fprintf(config,"n_sample_tests %lu\n",samples.size());
        fprintf(config,"input_pre %s\n",name.c_str());
        fprintf(config,"input_suf in\n");
        fprintf(config,"output_pre %s\n",name.c_str());
        fprintf(config,"output_suf out\n");
        fprintf(config,"time_limit %d\n",tl);
        fprintf(config,"memory_limit %d\n",ml);
        fprintf(config,"output_limit %d\n",ol);
        fprintf(config,"use_builtin_judger on\n");
        fprintf(config,"use_builtin_checker %s\n",chk.c_str());
        if(subtask.size()){
            fprintf(config,"n_subtasks %lu\n",subtask.size());
            for(unsigned i=0;i<subtask.size(); i++){
                fprintf(config,"subtask_end_%u %d\n",i+1,subtask[i].first);
                fprintf(config,"subtask_score_%u %d\n",i+1,subtask[i].second);
            }
        }
        fclose(config);
    }
};

#endif
