#pragma once

#include <exception>
#include <memory>
#include <unordered_map>
#include <vector>

template <class IdentifierType, class ProductType> struct DefaultFactoryError {
  struct Exception : public std::exception {
    Exception(const IdentifierType &unknownId) : unknownId(unknownId) {}
    virtual const char *what() const noexcept {
      return "Unknown object type passed to Factory.";
    }
    const IdentifierType GetId() { return unknownId; }

  private:
    IdentifierType unknownId;
  };

protected:
  std::unique_ptr<ProductType> OnUnknownType(const IdentifierType &id) {
    throw Exception(id);
  }
};

template <
    class AbstractProduct, typename IdentifierType, typename ProductCreator,
    template <typename, class> class FactoryErrorPolicy = DefaultFactoryError>
struct Factory : FactoryErrorPolicy<IdentifierType, AbstractProduct> {
  static Factory &Instance() {
    static Factory instance;
    return instance;
  }

  bool add(IdentifierType id, ProductCreator creator) {
    return associations
        .insert(
            typename AssocMap::value_type{std::move(id), std::move(creator)})
        .second;
  }
  std::vector<IdentifierType> registered() const {
    std::vector<IdentifierType> result;
    for (const auto &association : associations) {
      result.push_back(association.first);
    }
    return result;
  }
  bool remove(const IdentifierType &id) { return associations.erase(id) == 1; }
  template <class... Args>
  std::unique_ptr<AbstractProduct> create(const IdentifierType &id,
                                          Args &&... args) {
    auto i = associations.find(id);
    if (i != associations.end()) {
      return (i->second)(std::forward<Args>(args)...);
    }
    return this->OnUnknownType(id);
  }

private:
  Factory() = default;
  using AssocMap = std::unordered_map<IdentifierType, ProductCreator>;
  AssocMap associations;
};
