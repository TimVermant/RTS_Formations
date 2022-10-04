using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Formation : MonoBehaviour
{
    // Calculations
    [SerializeField] private int _unitsPerLine = 2;
    [SerializeField] private float _horizontalDistanceUnits = 4.0f; // Horizontal distance of units on the samel ine
    [SerializeField] private float _verticalDistanceUnits = 3.0f; // Vertical distance of units on the same line
    [SerializeField] private float _lineDistance = 5.0f; // Distance between 2 lines

    [SerializeField] private float _formationMaxSize = 20.0f;

    private Vector3 _desiredLeaderPos;
    private Vector3 _startPosition;

    private bool _retainLeaderUnit = false;

    private Unit _leaderUnit;
    private List<Unit> _units = new List<Unit>();


    private void Start()
    {
        var units = GetComponentsInChildren<Unit>();
        foreach (var unit in units)
        {
            _units.Add(unit);
        }
        _formationMaxSize = _units.Count;
    }

    private void Update()
    {
        if (Input.GetMouseButtonDown(0))
        {

            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
            if (Physics.Raycast(ray, out RaycastHit hit))
            {

                Vector3 targetPos = hit.point;
                targetPos.y = 0.0f;
                CreateFormation(targetPos);
            }
        }
    }

    private void CreateFormation(Vector3 startPos)
    {
        if (_leaderUnit)
            _leaderUnit.ResetUnit();
        GetClosestUnit(startPos);

        int index = _units.IndexOf(_leaderUnit);
        Swap(index, 0);
        _units[0].MakeLeader(startPos);

        CalculateDesiredPositions();


    }

    private void Swap(int index1, int index2)
    {
        Unit unit = _units[index1];
        _units[index1] = _units[index2];
        _units[index2] = unit;
    }

    private void GetClosestUnit(Vector3 startPos)
    {
        _startPosition = startPos;
        if (_retainLeaderUnit)
        {
            return;
        }

        float distance = 999.0f;
        Unit leaderUnit = null;
        foreach (Unit unit in _units)
        {
            if (distance > Vector3.Distance(_startPosition, unit.transform.position))
            {
                distance = Vector3.Distance(_startPosition, unit.transform.position);
                leaderUnit = unit;
            }
        }
        if (leaderUnit != null)
            _leaderUnit = leaderUnit;

    }

    private void CalculateDesiredPositions()
    {
        Vector3 position = new();
        Vector3 startPos = _startPosition;
        int offsetMultiplier;
        int index = 0;

        for (int i = 0; i < _formationMaxSize / _unitsPerLine; i++)
        {
            // Get middle front position of unit on line
            startPos = _startPosition;
            startPos.z -= _lineDistance * i;
            _units[index].MoveTowards(startPos);
            index++;
            offsetMultiplier = 1;
            for (int j = 1; j < _unitsPerLine; j++)//Start at index 1 because we already calculated the first position
            {
                // Reset to start position on line
                position = startPos;
                Debug.Log(position);

                float xOffset = offsetMultiplier * _horizontalDistanceUnits;
                float zOffset = offsetMultiplier * _verticalDistanceUnits;
                if (j % 2 != 0) // Odd number
                {
                    position.x -= xOffset;
                    position.z -= zOffset;
                }
                else // Even number
                {
                    position.x += xOffset;
                    position.z -= zOffset;
                    offsetMultiplier++; // After odd - even we increase the offsetmultiplier
                }

                _units[index].MoveTowards(position);
                index++;

            }

        }
    }

}
