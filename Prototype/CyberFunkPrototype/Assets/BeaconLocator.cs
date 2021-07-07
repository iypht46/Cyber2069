using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BeaconLocator : MonoBehaviour
{
    SpawnSystem spawner;

    private void Start()
    {
        spawner = GameObject.Find("Spawner").GetComponent<SpawnSystem>();
    }

    private void FixedUpdate()
    {
        if (spawner.CurrentBeacon != null)
        {
            this.transform.localRotation = Quaternion.Euler(0, 0, Vector2.SignedAngle(Vector2.right, spawner.CurrentBeacon.transform.position - this.transform.position));
        }
    }
}
