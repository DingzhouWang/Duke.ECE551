#include <cstdlib>
#include <sstream>
#include <string>

class Expression {
 public:
  Expression() {}
  virtual std::string toString() const = 0;
  virtual ~Expression() {}
};

class NumExpression : public Expression {
  long number;

 public:
  NumExpression(long n) : number(n) {}
  virtual std::string toString() const {
    std::stringstream answer;
    answer << number;
    return answer.str();
  }
  virtual ~NumExpression() {}
};

class PlusExpression : public Expression {
  Expression * lhs;
  Expression * rhs;

 public:
  PlusExpression(Expression * l, Expression * r) : lhs(l), rhs(r) {}
  virtual std::string toString() const {
    std::stringstream answer;
    answer << "(" << lhs->toString() << " + " << rhs->toString() << ")";
    return answer.str();
  }
  virtual ~PlusExpression() {
    delete lhs;
    delete rhs;
  }
};

class MinusExpression : public Expression {
  Expression * l;
  Expression * r;

 public:
  MinusExpression(Expression * lhs, Expression * rhs) : l(lhs), r(rhs) {}
  virtual std::string toString() const {
    std::stringstream answer;
    answer << "(" << l->toString() << " - " << r->toString() << ")";
    return answer.str();
  }
  virtual ~MinusExpression() {
    delete l;
    delete r;
  }
};

class TimesExpression : public Expression {
  Expression * l;
  Expression * r;

 public:
  TimesExpression(Expression * lhs, Expression * rhs) : l(lhs), r(rhs) {}
  virtual std::string toString() const {
    std::stringstream answer;
    answer << "(" << l->toString() << " * " << r->toString() << ")";
    return answer.str();
  }
  virtual ~TimesExpression() {
    delete l;
    delete r;
  }
};

class DivExpression : public Expression {
  Expression * l;
  Expression * r;

 public:
  DivExpression(Expression * lhs, Expression * rhs) : l(lhs), r(rhs) {}
  virtual std::string toString() const {
    std::stringstream answer;
    answer << "(" << l->toString() << " / " << r->toString() << ")";
    return answer.str();
  }
  virtual ~DivExpression() {
    delete l;
    delete r;
  }
};
