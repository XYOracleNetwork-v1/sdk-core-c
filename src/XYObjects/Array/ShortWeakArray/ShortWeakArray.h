#include "../../../XYResult.h"
#include "../../XYObject.h"
#include "../../XYObjectTypes.h"


/**
 * @brief Add Item from a Short Weak Array
 * 
 * @param self 
 * @param newItemHeader 
 * @param newItemLength 
 * @return XYResult_t 
 */
XYResult_t ShortWeakArray_add(XYObject_t* self,
                               XYObjectHeader_t newItemHeader,
                               int newItemLength);

/**
 * @brief Get Item from a Short Weak Array
 * 
 * @param self 
 * @param index 
 * @return XYResult_t 
 */
XYResult_t ShortWeakArray_get(XYObject_t *self,
                              int index);
