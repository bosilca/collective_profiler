//
// Copyright (c) 2020, NVIDIA CORPORATION. All rights reserved.
//
// See LICENSE.txt for license information
//

package scale

import (
	"sort"

	"github.com/gvallee/alltoallv_profiling/tools/internal/pkg/unit"
)

func mapFloat64sScaleDown(unitType int, unitScale int, values map[int]float64) (int, int, map[int]float64) {
	if unitScale == -1 {
		// Unit not recognized, nothing we can do
		return unitType, unitScale, values
	}

	newUnitScale := unitScale - 1
	if !unit.IsValidScale(unitType, newUnitScale) {
		// nothing we can do
		return unitType, unitScale, values
	}

	values = mapFloat64sCompute(DOWN, values)

	return unitType, newUnitScale, values
}

func mapFloat64sScaleUp(unitType int, unitScale int, values map[int]float64) (int, int, map[int]float64) {
	if unitScale == -1 {
		// Unit not recognized, nothing we can do
		return unitType, unitScale, values
	}

	newUnitScale := unitScale + 1
	if !unit.IsValidScale(unitType, newUnitScale) {
		// nothing we can do
		return unitType, unitScale, values
	}

	values = mapFloat64sCompute(UP, values)

	return unitType, newUnitScale, values
}

func mapFloat64sCompute(op int, values map[int]float64) map[int]float64 {
	newValues := make(map[int]float64)
	switch op {
	case DOWN:
		for key, val := range values {
			newValues[key] = val * 1000
		}
	case UP:
		for key, val := range values {
			newValues[key] = val / 1000
		}
	}
	return newValues
}

func MapFloat64s(unitID string, values map[int]float64) (string, map[int]float64) {
	var sortedValues []float64

	// Copy and sort the values to figure out what can be done
	for _, v := range values {
		sortedValues = append(sortedValues, v)
	}
	sort.Float64s(sortedValues)

	if len(sortedValues) >= 2 && sortedValues[0] >= 0 && sortedValues[len(values)-1] <= 1 {
		// We scale down all the values if possible

		// Translate the human reading unit into something we can inteprete
		unitType, unitScale := unit.FromString(unitID)

		unitType, unitScale, newValues := mapFloat64sScaleDown(unitType, unitScale, values)
		newUnitID := unit.ToString(unitType, unitScale)
		return MapFloat64s(newUnitID, newValues)
	}

	if len(sortedValues) > 0 && sortedValues[0] >= 1000 {
		// We scale up the value if possible

		// Translate the human reading unit into something we can inteprete
		unitType, unitScale := unit.FromString(unitID)

		unitType, unitScale, newValues := mapFloat64sScaleUp(unitType, unitScale, values)
		newUnitID := unit.ToString(unitType, unitScale)
		return MapFloat64s(newUnitID, newValues)
	}

	// Nothing to do, just return the same
	return unitID, values
}
