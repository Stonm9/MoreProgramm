
#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include <memory>
class IExpression {
 public:
  virtual int Calculate() const = 0;
  virtual ~IExpression() = default;
};

class Constant final : public IExpression {
  int value_;

 public:
  explicit Constant(int value) : value_(value) {
  }
  int Calculate() const final {
    return value_;
  }
};
//-----------------------------------------------------------
class IBinaryOperation : public IExpression {
  std::unique_ptr<IExpression> left_;
  std::unique_ptr<IExpression> right_;

 public:
  IBinaryOperation(std::unique_ptr<IExpression> left, std::unique_ptr<IExpression> right)
      : left_(std::move(left)), right_(std::move(right)){};
  int Calculate() const final {
    const auto x = left_->Calculate();
    const auto y = right_->Calculate();
    return Operator(x, y);
  }
  virtual int Operator(int x, int y) const = 0;
};

class Multiply final : public IBinaryOperation {
  using IBinaryOperation::IBinaryOperation;

 public:
  int Operator(int x, int y) const final {
    return x * y;
  }
};

class Sum final : public IBinaryOperation {
  using IBinaryOperation::IBinaryOperation;

 public:
  int Operator(int x, int y) const final {
    return x + y;
  }
};
class Subtract final : public IBinaryOperation {
 public:
  using IBinaryOperation::IBinaryOperation;

  int Operator(int x, int y) const final {
    return x - y;
  }
};
class Divide final : public IBinaryOperation {
 public:
  using IBinaryOperation::IBinaryOperation;

  int Operator(int x, int y) const final {
    return x / y;
  }
};
class Residual final : public IBinaryOperation {
 public:
  using IBinaryOperation::IBinaryOperation;

  int Operator(int x, int y) const final {
    return x % y;
  }
};
class Maximum final : public IBinaryOperation {
 public:
  using IBinaryOperation::IBinaryOperation;

  int Operator(int x, int y) const final {
    return x > y ? x : y;
  }
};

class Minimum final : public IBinaryOperation {
 public:
  using IBinaryOperation::IBinaryOperation;

  int Operator(int x, int y) const final {
    return x < y ? x : y;
  }
};
//------------------------------------------------------------
class IUnaryOperation : public IExpression {
  std::unique_ptr<IExpression> operand_;

 public:
  explicit IUnaryOperation(std::unique_ptr<IExpression> operand) : operand_(std::move(operand)){};
  int Calculate() const final {
    const auto x = operand_->Calculate();
    return Operation(x);
  }
  virtual int Operation(int x) const = 0;
};

class Square final : public IUnaryOperation {
 public:
  using IUnaryOperation::IUnaryOperation;  // забирает все конструкторы из базового класса
  int Operation(int x) const final {
    return x * x;
  }
};

class AbsoluteValue final : public IUnaryOperation {
 public:
  using IUnaryOperation::IUnaryOperation;
  int Operation(int x) const final {
    return x < 0 ? -x : x;
  }
};

class Minus final : public IUnaryOperation {
 public:
  using IUnaryOperation::IUnaryOperation;
  int Operation(int x) const final {
    return -x;
  }
};

class Plus final : public IUnaryOperation {
 public:
  using IUnaryOperation::IUnaryOperation;

  int Operation(int x) const final {
    return x;
  }
};
#endif