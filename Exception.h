//

#ifndef _EXCEPTION_
#define _EXCEPTION_

using namespace std;

class Exception : public exception
{
private:
protected:
public:
  std::string reason;
  ecs36b_Exception(std::string r) {
    if (r.empty())
      reason = "unknown";
    else
      reason = r;
  }
  ~ecs36b_Exception() throw () {};
  virtual const char* what() const throw () { return reason.c_str(); };
};

#endif /* _EXCEPTION_ */
