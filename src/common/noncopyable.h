namespace spx {

class noncopyable {
protected:
  noncopyable(const noncopyable &) = delete;
  noncopyable &operator=(const noncopyable &) = delete;

  noncopyable() = default;
  ~noncopyable() = default;
};

} // namespace spx
