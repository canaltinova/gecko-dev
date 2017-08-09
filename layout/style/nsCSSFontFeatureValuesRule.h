/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsCSSFontFeatureValuesRule_h
#define nsCSSFontFeatureValuesRule_h

#include "mozilla/css/Rule.h"
#include "mozilla/dom/CSSFontFeatureValuesRule.h"
#include "nsCSSValue.h"
#include "nsICSSDeclaration.h"
#include "nsIDOMCSSFontFeatureValuesRule.h"

class nsCSSFontFeatureValuesRule final : public mozilla::dom::CSSFontFeatureValuesRule
{
public:
  nsCSSFontFeatureValuesRule(uint32_t aLineNumber, uint32_t aColumnNumber)
    : mozilla::dom::CSSFontFeatureValuesRule(aLineNumber, aColumnNumber)
  {
  }

  nsCSSFontFeatureValuesRule(const nsCSSFontFeatureValuesRule& aCopy)
    // copy everything except our reference count
    : mozilla::dom::CSSFontFeatureValuesRule(aCopy),
      mFamilyList(aCopy.mFamilyList),
      mFeatureValues(aCopy.mFeatureValues)
  {
  }

  NS_DECL_ISUPPORTS_INHERITED

#ifdef DEBUG
  virtual void List(FILE* out = stdout, int32_t aIndent = 0) const override;
#endif
  virtual already_AddRefed<mozilla::css::Rule> Clone() const override;

  // nsIDOMCSSFontFeatureValuesRule interface
  NS_DECL_NSIDOMCSSFONTFEATUREVALUESRULE

  // WebIDL interface
  void GetCssTextImpl(nsAString& aCssText) const final;

  const mozilla::FontFamilyList& GetFamilyList() { return mFamilyList; }
  void SetFamilyList(const mozilla::FontFamilyList& aFamilyList);

  void AddValueList(int32_t aVariantAlternate,
                    nsTArray<gfxFontFeatureValueSet::ValueList>& aValueList);

  const nsTArray<gfxFontFeatureValueSet::FeatureValues>& GetFeatureValues()
  {
    return mFeatureValues;
  }

  virtual size_t SizeOfIncludingThis(mozilla::MallocSizeOf aMallocSizeOf) const override;

  // TODO: make them protected again?
  mozilla::FontFamilyList mFamilyList;
  nsTArray<gfxFontFeatureValueSet::FeatureValues> mFeatureValues;
protected:
  ~nsCSSFontFeatureValuesRule() {}
};

#endif // nsCSSFontFeatureValuesRule_h
