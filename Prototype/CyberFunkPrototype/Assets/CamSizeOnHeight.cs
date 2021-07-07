using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CamSizeOnHeight : MonoBehaviour
{
    [SerializeField] private float maxSize = 25;
    [SerializeField] private float minSize = 12;

    [SerializeField] private float ResetRate = 1;
    [SerializeField] private float ExpandRate = 0.5f;

    [SerializeField] private PlayerController player;
    private void FixedUpdate()
    {
        float size = Camera.main.orthographicSize;
        if (player.grounded)
        {
            size -= ResetRate * Time.fixedDeltaTime;
        }
        else
        {
            size += ExpandRate * Time.fixedDeltaTime;
        }

        size = Mathf.Clamp(size, minSize, maxSize);
        Camera.main.orthographicSize = size;
    }
}
