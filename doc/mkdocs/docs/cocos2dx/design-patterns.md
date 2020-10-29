# Design Patterns
## Service Locator
- Use *Service Locator* instead of *Singleton*.
- A service must:
    + Inherit `ee::IService`.
- Usages:
    + Use `ee::ServiceLocator::provide` to set a service.
    + Use `ee::ServiceLocator::resolve` to get a service.
## Observer
## Decorator