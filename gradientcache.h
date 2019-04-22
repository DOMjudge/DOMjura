/** \file gradientcache.h
  * \brief Contains a class for caching gradient.
  */

#ifndef GRADIENTCACHE_H
#define GRADIENTCACHE_H

#include <QObject>
#include <QHash>
#include <QPixmap>

#include "defines.h"

namespace DJ {
namespace View {

/** A pair of QColors.
  * Used for the QHash.
  */
struct ColorPair {
    /** Constructs a new, empty color pair.
      */
    ColorPair();
    /** Constructs a new color pair with the specified colors.
      * \param c1 The first color.
      * \param c2 The second color.
      */
    ColorPair(QColor c1, QColor c2) {
        this->color1 = c1;
        this->color2 = c2;
    }

    /** The first color.
      */
    QColor color1;
    /** The second color.
      */
    QColor color2;
};

/** Determines whether two colors are the same.
  * \param e1 The first color to compare.
  * \param e2 The second color to compare.
  * \return true if and only if both colors are equal.
  */
inline bool operator==(const ColorPair &e1, const ColorPair &e2) {
    return e1.color1 == e2.color1 && e1.color2 == e2.color2;
}

/** Returns the hash index for the given color pair.
  * \param key The pair to compute the has of.
  * \return The hash index for this color pair.
  */
inline uint qHash(const ColorPair &key) {
    QString str;
    str += key.color1.name();
    str += key.color2.name();
    return qHash(str);
}

/** A class to cache gradients. This is usefull, because
  * this speeds up rendering a lot when using the same
  * gradient over and over, which this application does.
  * \note This class is a singleton class.
  */
class GradientCache : public QObject {
    Q_OBJECT
public:
    /** Gets the singleton gradient cache.
      */
    static GradientCache *getInstance();

    /** Clears the cache.
      * \note This function needs to be called when
      * the settings are changed.
      */
    void clearCache();
    /** Sets the number of problems. This is used to
      * calculate the problem width.
      * \note This resets the problem gradient caches.
      */
    void setNumProbs(int n);
    /** Sets the height of the problem gradients.
      * \note This resets the problem gradient caches.
      */
    void setHeight(int height);
    /** Gets a pixmap containing a problem gradient for the given colors.
      * If the gradient is already computed, it just returns it.
      * Otherwise, it is first calculated and saved for later use.
      * \param color1 The first color to use.
      * \param color2 The second color to use.
      * \return A pixmap containing the gradient.
      */
    QPixmap getColorGradient(QColor color1, QColor color2);
    /** Gets a pixmap containing a problem gradient for the given colors in the highlighted state.
      * If the gradient is already computed, it just returns it.
      * Otherwise, it is first calculated and saved for later use.
      * \param color1 The first color to use.
      * \param color2 The second color to use.
      * \return A pixmap containing the gradient.
      */
    QPixmap getColorGradientHighlighted(QColor color1, QColor color2);
    /** Gets a pixmap containing a gradient for the given medal.
      * If the gradient is already computed, it just returns it.
      * Otherwise, it is first calculated and saved for later use.
      * \param medal The medal to calculate the gradient for.
      * \return A pixmap containing the gradient.
      */
    QPixmap getMedalGradient(Medal medal);
    /** Gets a pixmap containing a gradient for an odd, even or highlighted row.
      * If the gradient is already computed, it just returns it.
      * Otherwise, it is first calculated and saved for later use.
      * \param oddEvenHighlighted If 0, returns the even gradient.
      * If 1, returns the odd gradient. If 2, returns the highlighted gradient.
      * \return A pixmap containing the gradient.
      */
    QPixmap getOddEvenHighlightedGradient(int oddEvenHighlighted);

protected:
    GradientCache();

private:
    static GradientCache *instance;

    int numProbs;
    int height;
    QHash<ColorPair, QPixmap> problemGradient;
    QHash<ColorPair, QPixmap> problemGradientHighlighted;
    QHash<Medal, QPixmap> medalGradient;
    QHash<int, QPixmap> oddEvenHighlightGradient;

};

} // namespace View
} // namespace DJ

#endif // GRADIENTCACHE_H
