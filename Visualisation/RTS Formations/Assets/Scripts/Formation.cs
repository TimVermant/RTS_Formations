using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Formation : MonoBehaviour
{
    // Calculations
    [SerializeField] private int _unitsPerLine;
    [SerializeField] private float _horizontalDistanceUnits;
    [SerializeField] private float _verticalDistanceUnits;
    [SerializeField] private float _lineDistance;

    private Vector3 _desiredLeaderPos;

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
        Debug.Log(units.Length);
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

        
    }

    private void Swap(int index1, int index2)
    {
        Unit unit = _units[index1];
        _units[index1] = _units[index2];
        _units[index2] = unit;
    }

    private void GetClosestUnit(Vector3 startPos)
    {
        _desiredLeaderPos = startPos;
        if (_retainLeaderUnit)
        {
            return;
        }

        float distance = 999.0f;
        Unit leaderUnit = null;
        foreach (Unit unit in _units)
        {
            if (distance > Vector3.Distance(_desiredLeaderPos, unit.transform.position))
            {
                distance = Vector3.Distance(_desiredLeaderPos, unit.transform.position);
                leaderUnit = unit;
            }
        }
        if (leaderUnit != null)
            _leaderUnit = leaderUnit;

    }



}
