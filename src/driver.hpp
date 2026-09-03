#ifndef DRIVER_HPP
#define DRIVER_HPP

#include "common.hpp"

namespace driver {

template<template<bool> class Driver> concept DRIVER = requires {
	{ Driver<true>::programNames() } noexcept -> std::convertible_to<std::pair<std::string, std::string> >;
	{ Driver<true>::addArguments() } noexcept;
	{ Driver<true>::getStepwiseColorSharedConstData() } noexcept;
	{ std::visit([]<typename StepwiseColorSharedConstData>(StepwiseColorSharedConstData const& stepwiseColorSharedConstData) {}, Driver<true>::getStepwiseColorSharedConstData()) };
};

template<template<bool> class Driver> concept ANNOTATION_READY = requires {
	requires DRIVER<Driver>;
	requires quadripartition_support::BRANCH_ANNOTATION<typename Driver<true>::BranchAnnotation, typename std::variant_alternative_t<0, typename Driver<true>::DataClasses>::ParentClass>;
	{ Driver<true>::DEFAULT_SUPPORT_ANNOTATION() } noexcept -> std::convertible_to<std::string>;
};

template<template<bool> class Driver, class Color> requires (!ANNOTATION_READY<Driver>) std::string const annotate(typename Color::SharedConstData const& data, common::AnnotatedBinaryTree& tree, size_t nThreads, int verbose) {
	return "";
}

template<template<bool> class Driver, class Color> requires ANNOTATION_READY<Driver> std::string const annotate(typename Color::SharedConstData const& data, common::AnnotatedBinaryTree& tree, size_t nThreads, int verbose) {
	return Driver<true>::BranchAnnotation::template annotate<Color>(Driver<true>::DEFAULT_SUPPORT_ANNOTATION(), data, tree, nThreads, 0);
}

};

#endif // !DRIVER_HPP
